using System.Drawing;
using System.Windows.Forms;

namespace MtgDatabaseClient.userControls
{
    public partial class UserControlWaiting : UserControl
    {
        public UserControlWaiting(string text = "Cargando", float fontSize = 50)
        {
            InitializeComponent();
            this.Dock = DockStyle.Fill;
            setText(text);
            setFontSize(fontSize); ;
        }

        public void setText(string text)
        {
            labelText.Text = text + "\n...";
        }

        public void setFontSize(float fontSize)
        {
            labelText.Font = new Font(labelText.Font.FontFamily, fontSize, labelText.Font.Style);
        }

        private void UserControlWaiting_Load(object sender, System.EventArgs e) {
            BringToFront();
        }
    }
}
