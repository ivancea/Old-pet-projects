using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Printing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tickets {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        int printingIndex = 0;
        List<DataGridViewRow> printingData = new List<DataGridViewRow>();
        PrintDocument printDocument = new PrintDocument();

        private void print_Click(object sender, EventArgs e) {

            print.Enabled = false;

            bool any = false;
            foreach(DataGridViewRow row in data.Rows)
                if(printable(row)) {
                    any = true;
                    break;
                }
            if(!any) {
                MessageBox.Show("No hay ninguna fila marcada o están incompletas");
                print.Enabled = true;
                return;
            }

            printDocument = new PrintDocument();
            printDocument.PrintPage += new PrintPageEventHandler(printDocument_PrintPage);
            printDocument.BeginPrint += new PrintEventHandler(printDocument_BeginPrint);
            printDocument.EndPrint += new PrintEventHandler(printDocument_EndPrint);
            printDocument.OriginAtMargins = true;
            printDocument.DocumentName = "Etiquetas";
            PrintDialog printDialog = new PrintDialog();
            printDialog.Document = printDocument;
            if(printDialog.ShowDialog() == DialogResult.OK) {
                printDocument.Print();
            }
            printingIndex = 0;
            printingData.Clear();
            print.Enabled = true;
        }

        private void printDocument_BeginPrint(object sender, PrintEventArgs e) {
            printDocument.DefaultPageSettings.Landscape = false;
            printDocument.DefaultPageSettings.Margins.Top = 20;
            printDocument.DefaultPageSettings.Margins.Right = 65;
            printDocument.DefaultPageSettings.Margins.Bottom = 20;
            printDocument.DefaultPageSettings.Margins.Left = 57;

            printingIndex = 0;
            printingData.Clear();

            foreach(DataGridViewRow row in data.Rows)
                if(printable(row))
                    printingData.Add(row);
        }

        private void printDocument_EndPrint(object sender, PrintEventArgs e) {
            printingIndex = 0;
            printingData.Clear();
        }

        private void printDocument_PrintPage(object sender, PrintPageEventArgs e) {
            int incX = e.MarginBounds.Height / 8;
            int incY = e.MarginBounds.Width / 2;
            for(int i = 0; i < 16 && printingIndex < printingData.Count; i++, printingIndex++) {
                if(i%2==0)
                    drawFormatted(e.Graphics,
                                  printingData[printingIndex].Cells["codigo"].Value.ToString(),
                                  printingData[printingIndex].Cells["articulo"].Value.ToString(),
                                  double.Parse(printingData[printingIndex].Cells["pvp"].Value.ToString()),
                                  5, (i/2)*incX+5,
                                  incY - 10, incX - 11);
                else
                    drawFormatted(e.Graphics,
                                  printingData[printingIndex].Cells["codigo"].Value.ToString(),
                                  printingData[printingIndex].Cells["articulo"].Value.ToString(),
                                  double.Parse(printingData[printingIndex].Cells["pvp"].Value.ToString()),
                                  incY + 5,(i/2)*incX+5,
                                  incY-10, incX - 11);
            }
            e.HasMorePages = (printingIndex < printingData.Count);
        }

        private void drawFormatted(Graphics g, string codigo, string articulo, double pvp_double, int x, int y, int width, int height) {
            string pvp_entero = Convert.ToInt32(Math.Floor(pvp_double)).ToString();
            string pvp_decimal = String.Format("{0:#.00}", Math.Abs(pvp_double-Convert.ToInt32(Math.Floor(pvp_double))))+"€";

            Font f = new Font("Arial", 30, GraphicsUnit.Pixel);
            while(g.MeasureString(articulo, f).Width >= width && f.Size>1)
                f = new Font("Arial", f.Size - 1, GraphicsUnit.Pixel);
            g.DrawString(articulo, f, Brushes.Black, new RectangleF(x, y, width, height / 3 - 1));
            int articuloSize = Convert.ToInt32(g.MeasureString(articulo, f).Height);

            f = new Font("Arial", 50, GraphicsUnit.Pixel);
            while(g.MeasureString(pvp_entero, f).Height < height - 40)
                f = new Font("Arial", f.Size + 1, GraphicsUnit.Pixel);
            Font f2 = new Font("Arial", f.Size / 2, GraphicsUnit.Pixel);
            int offsetRight = Convert.ToInt32(g.MeasureString(pvp_entero, f).Width + g.MeasureString(pvp_decimal, f2).Width);
            g.DrawString(pvp_entero, f, Brushes.Black, new RectangleF(x + width - offsetRight + 20, y + 40, offsetRight, height - 40));
            g.DrawString(pvp_decimal, f2, Brushes.Black, new RectangleF(x + width - offsetRight + g.MeasureString(pvp_entero, f).Width, y + 20 + g.MeasureString(pvp_decimal, f2).Height, offsetRight, height - 40));
            
            f = new Font("Arial", 10, GraphicsUnit.Pixel);
            g.DrawString(codigo, f, Brushes.Black, new RectangleF(x, y+height-20, width, 30));
        }

        private void Form1_Load(object sender, EventArgs e) {
            this.articulosTableAdapter.Fill(this.etiquetas_artículosDataSet.Articulos);
        }

        private void Form1_Closing(object sender, FormClosingEventArgs e) {
            DialogResult result = MessageBox.Show("¿Quieres guardar los cambios?","Guardar",MessageBoxButtons.YesNoCancel);
            switch(result) {
                case DialogResult.Cancel:
                    e.Cancel = true;
                    break;
                case DialogResult.Yes:
                    result = DialogResult.Retry;
                    while(!saveDB() && result == DialogResult.Retry) {
                        e.Cancel = true;
                        result = MessageBox.Show("Error guardando cambios...", "Error", MessageBoxButtons.RetryCancel);
                    }
                    break;
            }
        }

        private void eraseMarks_Click(object sender, EventArgs e) {
            foreach(DataGridViewRow row in data.Rows){
                row.Cells["imprimir"].Value = false;
            }
        }

        private void data_CellEndEdit(object sender, DataGridViewCellEventArgs e) {
            if(e.RowIndex < 0 || e.RowIndex >= data.RowCount)
                return;
            parseRow(data.Rows[e.RowIndex]);
            if(e.ColumnIndex == data.Columns["pvp"].Index)
                data.Rows[e.RowIndex].Cells["imprimir"].Value = true;
        }

        private void data_CellClick(object sender, DataGridViewCellEventArgs e) {
            if(e.RowIndex < 0 || e.RowIndex >= data.RowCount)
                return;
            parseRow(data.Rows[e.RowIndex]);
        }

        private void data_CellContentClick(object sender, DataGridViewCellEventArgs e) {
            if(e.RowIndex < 0 || e.RowIndex >= data.RowCount || e.ColumnIndex!=data.ColumnCount-1)
                return;
            if(!secureMode.Checked ||
               MessageBox.Show("¿Seguro que quieres eliminar la fila?",
                               "Confirmación",
                               MessageBoxButtons.YesNo) == DialogResult.Yes) {
                data.Rows.RemoveAt(e.RowIndex);
                data.Update();
            }
        }

        private void calcularPrecios_Click(object sender, EventArgs e) {
            foreach(DataGridViewRow row in data.Rows)
                parseRow(row);
        }

        private void parseRow(DataGridViewRow row) {
            if(row.Cells["compra"].Value != null
            && row.Cells["iva"].Value != null
            && row.Cells["margen"].Value != null
            &&  row.Cells["compra"].Value != DBNull.Value
            && row.Cells["iva"].Value != DBNull.Value
            && row.Cells["margen"].Value != DBNull.Value) {
                double t = double.Parse(row.Cells["compra"].Value.ToString());
                t += double.Parse(row.Cells["iva"].Value.ToString())*t/100;
                t += double.Parse(row.Cells["margen"].Value.ToString()) * t/100;
                row.Cells["pvpCalculado"].Value = t;
            }
        }

        private void save_Click(object sender, EventArgs e) {
            if(!saveDB())
                MessageBox.Show("Error guardando cambios...", "Error", MessageBoxButtons.OK);
        }

        private void calcularHojas_Click(object sender, EventArgs e) {
            int n = 0;
            foreach(DataGridViewRow row in data.Rows) 
                if(printable(row))
                    ++n;
            int hojas = (n==0?0:(n-1)/16+1);
            MessageBox.Show("Se necesita" + (hojas > 1 ? "n" : "") + " " + hojas + " hoja" + (hojas > 1 ? "s" : "") + " (" + n + " artículos)");
        }

        private bool printable(DataGridViewRow row){
            object t = row.Cells["imprimir"].Value;
            if(t == null || t.ToString() != "True")
                return false;
            t = row.Cells["codigo"].Value;
            if(t==null)
                return false;
            t = row.Cells["articulo"].Value;
            if(t==null)
                return false;
            t = row.Cells["pvp"].Value;
            if(t==null)
                return false;
            double _;
            if(!double.TryParse(row.Cells["pvp"].Value.ToString(), out _))
                return false;
            return true;
        }

        private bool saveDB() {
            bool ret = true;
            try {
                Validate();
                articulosBindingSource.EndEdit();
                articulosTableAdapter.Update(etiquetas_artículosDataSet.Articulos);
            } catch {
                ret = false;
            }
            return ret;
        }

        private void data_DataError(object sender, DataGridViewDataErrorEventArgs e) {
            e.ThrowException = false;
            MessageBox.Show("Datos incorrectos");
        }

    }
}
