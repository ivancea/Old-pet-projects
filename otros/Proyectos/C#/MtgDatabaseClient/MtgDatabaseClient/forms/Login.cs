using System;
using System.Collections.Generic;
using System.Windows.Forms;
using MtgDatabaseClient.userControls;

namespace MtgDatabaseClient.forms{
    public partial class Login : Form {

        public bool exit = true;

        public Login() {
            InitializeComponent();
            CenterToScreen();

            AppConfig.refresh();

            string server = AppConfig.getSetting("server"),
                   userid = AppConfig.getSetting("userid"),
                   database = AppConfig.getSetting("database");
            if (!String.IsNullOrWhiteSpace(server)) {
                textServer.Text = server;
                textServer.Enabled = false;
            }
            if (!String.IsNullOrWhiteSpace(userid)) {
                textUserid.Text = userid;
                textUserid.Enabled = false;
            }
            if (!String.IsNullOrWhiteSpace(database)) {
                textDatabase.Text = database;
                textDatabase.Enabled = false;
            }

        }

        private void buttonAccess_Click(object sender, EventArgs e) {
            UserControlWaiting wait = new UserControlWaiting("Validando", 40);
            Controls.Add(wait);
            Refresh();
            labelError.Text = "";
            //this.Enabled = false;
            DataConnection.server = textServer.Text;
            DataConnection.userid = textUserid.Text;
            DataConnection.password = textPassword.Text;
            DataConnection.database = textDatabase.Text;
            DataConnection db = new DataConnection();

            bool endForm = false;
            if (db.testConnection()) {
                AppConfig.setSettings(new Dictionary<string, string> { { "server",   textServer.Text   },
                                                                       { "userid",   textUserid.Text   },
                                                                       { "database", textDatabase.Text } });
                AppConfig.refresh();
                endForm = true;
            } else {
                switch (db.getLastErrorCode()) {
                    case 0: labelError.Text = "No se puede conectar al servidor";
                        break;
                    case 1045: labelError.Text = "Usuario o contraseña inválidos";
                        break;
                    default: labelError.Text = "Error en la conexión (Código " + Convert.ToString(db.getLastErrorCode()) + ")";
                        break;
                }
            }
            //this.Enabled = true;
            Controls.Remove(wait);
            if (endForm) {
                exit = false;
                Close();
            }
        }

        private void buttonBlock_Click(object sender, EventArgs e) {
            if (textServer.Enabled) {
                buttonBlock.Text = "Desbloquear";
                textServer.Enabled =
                textUserid.Enabled =
                textDatabase.Enabled = false;
            } else {
                buttonBlock.Text = "Bloquear";
                textServer.Enabled =
                textUserid.Enabled =
                textDatabase.Enabled = true;
            }
        }
    }
}
