using System;
using System.Collections.Generic;
using System.Configuration;
using System.Net;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using MtgDatabaseClient.userControls;

namespace MtgDatabaseClient.forms {
    public partial class ScanForm : Form {

        public ScanForm() {
            InitializeComponent();
        }

        private void ScanForm_Load(object sender, EventArgs e) {
            int page = AppConfig.getSettingInt("lastPage");
            if (page >= 1)
                textInitialPage.Text = page.ToString();
        }

        private const string searchUri =
            "http://gatherer.wizards.com/Pages/Search/Default.aspx?action=advanced&output=checklist&special=true&cmc=+>=[0]&page=";
        private const string cardUri =
                    "http://gatherer.wizards.com/Pages/Card/Details.aspx?multiverseid=";

        private long totalScanned = 0;
        private Thread th;

        private static volatile bool threadMustStop = false;

        private void buttonStartScanning_Click(object sender, System.EventArgs e) {
            buttonStartScanning.Enabled = false;
            totalScanned = 0;
            labelTotalScanned.Text = "0";
            threadMustStop = false;
            int initialPage;
            if (!int.TryParse(textInitialPage.Text, out initialPage))
                initialPage = 1;
            th = new Thread(() => threadFunction(this, initialPage));
            th.Start();
        }

        private void ScanForm_FormClosing(object sender, FormClosingEventArgs e) {
            int page;
            if (int.TryParse(labelActualPage.Text, out page))
                AppConfig.setSetting("lastPage", page.ToString());
            if (th != null && th.IsAlive) {
                threadMustStop = true;
                UserControlWaiting wait = new UserControlWaiting();
                wait.setText("Waiting thread");
                Controls.Add(wait);
                Refresh();
                th.Join();
            }
        }

        private void actualize(CardData card) {
            totalScanned++;
            labelTotalScanned.Text = totalScanned.ToString();
            labelActualId.Text = card.id.ToString();
            labelActualName.Text = card.name;
            labelActualType.Text = card.type;
            labelActualRarity.Text = card.rarity;
        }

        private static void threadFunction(ScanForm form, int initialPage) {
            int lastPageHash = 0;
            int page = initialPage;

            while (!threadMustStop) {
                string body;
                using (var client = new WebClient()) {
                    body = client.DownloadString(searchUri + page);
                }
                int t = body.GetHashCode();
                if (lastPageHash == t) {
                    threadMustStop = true;
                    break;
                }
                
                lastPageHash = t;

                form.BeginInvoke(new Action(() => {
                    form.labelActualPage.Text = page.ToString();
                }));

                int pos = 0;
                long lastId = -1;
                while (!threadMustStop) {
                    pos = body.IndexOf("?multiverseid=", pos + 1, StringComparison.Ordinal);
                    if (pos == -1)
                        break;
                    int pos2 = body.IndexOf("\"", pos, StringComparison.Ordinal);
                    try {
                        long id = Convert.ToInt64(body.Substring(pos + 14, pos2 - pos - 14));
                        if (lastId != id) {
                            searchById(form, id);
                            lastId = id;
                        }
                    } catch (Exception exc) {
                        if (MessageBox.Show("Error: " + exc.Message, "Stop scanning?", MessageBoxButtons.YesNo)
                            == DialogResult.Yes) {
                            threadMustStop = true;
                        }
                    }
                }
                page++;
            }
            form.BeginInvoke(new Action(() => {
                form.buttonStartScanning.Enabled = true;
            }));
        }

        private static void searchById(ScanForm form, long id) {

            string body;
            using (var client = new WebClient()) {
                client.Encoding = Encoding.UTF8;
                body = client.DownloadString(cardUri + id);
            }

            int pos = body.IndexOf("<div class=\"value\">", StringComparison.Ordinal); // IGNORED
            int pos2 = body.IndexOf("</div>", pos, StringComparison.Ordinal);
                
            pos = body.IndexOf("<div class=\"value\">", pos2, StringComparison.Ordinal);
            pos2 = body.IndexOf("</div>", pos, StringComparison.Ordinal);

            var name = body.Substring(pos + 19, pos2 - pos - 19).Trim(" \n\r\t".ToCharArray());

            pos = body.IndexOf("Types:", pos2, StringComparison.Ordinal);
            pos = body.IndexOf("<div class=\"value\">", pos, StringComparison.Ordinal);
            pos2 = body.IndexOf("</div>", pos, StringComparison.Ordinal);

            var type = body.Substring(pos + 19, pos2 - pos - 19).Trim(" \n\r\t".ToCharArray());

            pos = body.IndexOf("</span>", pos2, StringComparison.Ordinal); // Rarity is inside a span
            pos2 = body.LastIndexOf(">", pos, StringComparison.Ordinal);

            var rarity = body.Substring(pos2+1, pos - pos2 - 1).Trim(" \n\r\t".ToCharArray());


            CardData card = new CardData {
                id = id,
                name = name,
                type = type,
                rarity = rarity
            };
            form.BeginInvoke(new Action(() => {
                form.actualize(card);
            }));

            DataConnection conn = new DataConnection();

            int n = conn.executeNonQuery(@"INSERT IGNORE INTO tbCards (multiverseId,name,type,rarity)
                                           VALUES (@id,@name,@type,@rarity)",
                                           new Dictionary<String, Object> {
                                               {"@id", id},
                                               {"@name", name},
                                               {"@type", type},
                                               {"@rarity", rarity}
                                           });
            if (n < 0) {
                if (MessageBox.Show("Error: " + conn.getLastError(), "Stop scanning?", MessageBoxButtons.YesNo)
                    == DialogResult.Yes) {
                    threadMustStop = true;
                }
            }

        }

        private void buttonSearch_Click(object sender, EventArgs e) {
            long id;
            if (long.TryParse(textCardId.Text, out id)) {
                webCard.Navigate(cardUri + id);
            } else MessageBox.Show("Invalid id");
        }
    }
}
