using System;
using System.Globalization;
using System.Threading;
using System.Windows.Forms;
using MtgDatabaseClient.forms;

namespace MtgDatabaseClient {
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main() {
            CultureInfo customCulture = (CultureInfo)Thread.CurrentThread.CurrentCulture.Clone();
            customCulture.NumberFormat.NumberDecimalSeparator = ".";
            Thread.CurrentThread.CurrentCulture = customCulture;

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            AppConfig.refresh();

            using (Login login = new Login())
            {
                Application.Run(login);

                if (login.exit)
                    return;
            }

            using (ScanForm baseForm = new ScanForm()){
                Application.Run(baseForm);
            }

        }
    }
}
