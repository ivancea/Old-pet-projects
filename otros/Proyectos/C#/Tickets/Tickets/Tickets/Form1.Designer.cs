namespace Tickets
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            this.print = new System.Windows.Forms.Button();
            this.data = new System.Windows.Forms.DataGridView();
            this.articulosBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.etiquetas_artículosDataSet = new Tickets.Etiquetas_artículosDataSet();
            this.articulosTableAdapter = new Tickets.Etiquetas_artículosDataSetTableAdapters.ArticulosTableAdapter();
            this.eraseMarks = new System.Windows.Forms.Button();
            this.save = new System.Windows.Forms.Button();
            this.calcularHojas = new System.Windows.Forms.Button();
            this.calcularPrecios = new System.Windows.Forms.Button();
            this.secureMode = new System.Windows.Forms.CheckBox();
            this.codigo = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.fAMILIADataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.articulo = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.compra = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.iva = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.margen = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.pvpCalculado = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.pvp = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.imprimir = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.Column1 = new System.Windows.Forms.DataGridViewButtonColumn();
            ((System.ComponentModel.ISupportInitialize)(this.data)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.articulosBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.etiquetas_artículosDataSet)).BeginInit();
            this.SuspendLayout();
            // 
            // print
            // 
            this.print.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.print.Location = new System.Drawing.Point(587, 266);
            this.print.Name = "print";
            this.print.Size = new System.Drawing.Size(126, 44);
            this.print.TabIndex = 0;
            this.print.Text = "Imprimir";
            this.print.UseVisualStyleBackColor = true;
            this.print.Click += new System.EventHandler(this.print_Click);
            // 
            // data
            // 
            this.data.AllowUserToResizeRows = false;
            this.data.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.data.AutoGenerateColumns = false;
            this.data.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCellsExceptHeaders;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.data.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.data.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.data.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.codigo,
            this.fAMILIADataGridViewTextBoxColumn,
            this.articulo,
            this.compra,
            this.iva,
            this.margen,
            this.pvpCalculado,
            this.pvp,
            this.imprimir,
            this.Column1});
            this.data.DataSource = this.articulosBindingSource;
            this.data.EditMode = System.Windows.Forms.DataGridViewEditMode.EditOnEnter;
            this.data.Location = new System.Drawing.Point(0, 0);
            this.data.MinimumSize = new System.Drawing.Size(840, 0);
            this.data.Name = "data";
            this.data.Size = new System.Drawing.Size(955, 260);
            this.data.TabIndex = 1;
            this.data.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.data_CellClick);
            this.data.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.data_CellContentClick);
            this.data.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.data_CellEndEdit);
            this.data.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.data_DataError);
            // 
            // articulosBindingSource
            // 
            this.articulosBindingSource.DataMember = "Articulos";
            this.articulosBindingSource.DataSource = this.etiquetas_artículosDataSet;
            // 
            // etiquetas_artículosDataSet
            // 
            this.etiquetas_artículosDataSet.DataSetName = "Etiquetas_artículosDataSet";
            this.etiquetas_artículosDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // articulosTableAdapter
            // 
            this.articulosTableAdapter.ClearBeforeFill = true;
            // 
            // eraseMarks
            // 
            this.eraseMarks.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.eraseMarks.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.eraseMarks.Location = new System.Drawing.Point(276, 302);
            this.eraseMarks.Name = "eraseMarks";
            this.eraseMarks.Size = new System.Drawing.Size(142, 37);
            this.eraseMarks.TabIndex = 2;
            this.eraseMarks.Text = "Borrar marcas";
            this.eraseMarks.UseVisualStyleBackColor = true;
            this.eraseMarks.Click += new System.EventHandler(this.eraseMarks_Click);
            // 
            // save
            // 
            this.save.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.save.Location = new System.Drawing.Point(806, 279);
            this.save.Name = "save";
            this.save.Size = new System.Drawing.Size(121, 60);
            this.save.TabIndex = 3;
            this.save.Text = "Guardar datos";
            this.save.UseVisualStyleBackColor = true;
            this.save.Click += new System.EventHandler(this.save_Click);
            // 
            // calcularHojas
            // 
            this.calcularHojas.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.calcularHojas.Location = new System.Drawing.Point(587, 316);
            this.calcularHojas.Name = "calcularHojas";
            this.calcularHojas.Size = new System.Drawing.Size(126, 23);
            this.calcularHojas.TabIndex = 4;
            this.calcularHojas.Text = "Calcular hojas";
            this.calcularHojas.UseVisualStyleBackColor = true;
            this.calcularHojas.Click += new System.EventHandler(this.calcularHojas_Click);
            // 
            // calcularPrecios
            // 
            this.calcularPrecios.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.calcularPrecios.Location = new System.Drawing.Point(424, 302);
            this.calcularPrecios.Name = "calcularPrecios";
            this.calcularPrecios.Size = new System.Drawing.Size(142, 37);
            this.calcularPrecios.TabIndex = 5;
            this.calcularPrecios.Text = "Calcular todos los PVP";
            this.calcularPrecios.UseVisualStyleBackColor = true;
            this.calcularPrecios.Click += new System.EventHandler(this.calcularPrecios_Click);
            // 
            // secureMode
            // 
            this.secureMode.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.secureMode.AutoSize = true;
            this.secureMode.Checked = true;
            this.secureMode.CheckState = System.Windows.Forms.CheckState.Checked;
            this.secureMode.Location = new System.Drawing.Point(0, 339);
            this.secureMode.Name = "secureMode";
            this.secureMode.Size = new System.Drawing.Size(150, 17);
            this.secureMode.TabIndex = 6;
            this.secureMode.Text = "Eliminación segura de filas";
            this.secureMode.UseVisualStyleBackColor = true;
            // 
            // codigo
            // 
            this.codigo.DataPropertyName = "CODIGO";
            this.codigo.Frozen = true;
            this.codigo.HeaderText = "CODIGO";
            this.codigo.Name = "codigo";
            // 
            // fAMILIADataGridViewTextBoxColumn
            // 
            this.fAMILIADataGridViewTextBoxColumn.DataPropertyName = "FAMILIA";
            this.fAMILIADataGridViewTextBoxColumn.Frozen = true;
            this.fAMILIADataGridViewTextBoxColumn.HeaderText = "FAMILIA";
            this.fAMILIADataGridViewTextBoxColumn.Name = "fAMILIADataGridViewTextBoxColumn";
            // 
            // articulo
            // 
            this.articulo.DataPropertyName = "ARTICULO";
            this.articulo.Frozen = true;
            this.articulo.HeaderText = "ARTICULO";
            this.articulo.Name = "articulo";
            // 
            // compra
            // 
            this.compra.DataPropertyName = "COMPRA";
            dataGridViewCellStyle2.Format = "N2";
            this.compra.DefaultCellStyle = dataGridViewCellStyle2;
            this.compra.Frozen = true;
            this.compra.HeaderText = "COMPRA";
            this.compra.Name = "compra";
            // 
            // iva
            // 
            this.iva.DataPropertyName = "IVA";
            this.iva.Frozen = true;
            this.iva.HeaderText = "IVA";
            this.iva.Name = "iva";
            this.iva.Width = 60;
            // 
            // margen
            // 
            this.margen.DataPropertyName = "MARGEN";
            this.margen.Frozen = true;
            this.margen.HeaderText = "MARGEN";
            this.margen.Name = "margen";
            this.margen.Width = 60;
            // 
            // pvpCalculado
            // 
            this.pvpCalculado.DataPropertyName = "PVP calculado";
            dataGridViewCellStyle3.Format = "N2";
            dataGridViewCellStyle3.NullValue = null;
            this.pvpCalculado.DefaultCellStyle = dataGridViewCellStyle3;
            this.pvpCalculado.Frozen = true;
            this.pvpCalculado.HeaderText = "PVP calculado";
            this.pvpCalculado.Name = "pvpCalculado";
            // 
            // pvp
            // 
            this.pvp.DataPropertyName = "PVP";
            dataGridViewCellStyle4.Format = "N2";
            this.pvp.DefaultCellStyle = dataGridViewCellStyle4;
            this.pvp.Frozen = true;
            this.pvp.HeaderText = "PVP";
            this.pvp.Name = "pvp";
            // 
            // imprimir
            // 
            this.imprimir.DataPropertyName = "Imprimir";
            this.imprimir.Frozen = true;
            this.imprimir.HeaderText = "Imprimir";
            this.imprimir.Name = "imprimir";
            this.imprimir.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            this.imprimir.Width = 40;
            // 
            // Column1
            // 
            this.Column1.Frozen = true;
            this.Column1.HeaderText = "X";
            this.Column1.Name = "Column1";
            this.Column1.Width = 20;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(956, 357);
            this.Controls.Add(this.secureMode);
            this.Controls.Add(this.calcularPrecios);
            this.Controls.Add(this.calcularHojas);
            this.Controls.Add(this.save);
            this.Controls.Add(this.eraseMarks);
            this.Controls.Add(this.data);
            this.Controls.Add(this.print);
            this.Name = "Form1";
            this.Text = "Etiquetas";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_Closing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.data)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.articulosBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.etiquetas_artículosDataSet)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button print;
        private System.Windows.Forms.DataGridView data;
        private Etiquetas_artículosDataSet etiquetas_artículosDataSet;
        private System.Windows.Forms.BindingSource articulosBindingSource;
        private Etiquetas_artículosDataSetTableAdapters.ArticulosTableAdapter articulosTableAdapter;
        private System.Windows.Forms.Button eraseMarks;
        private System.Windows.Forms.Button save;
        private System.Windows.Forms.Button calcularHojas;
        private System.Windows.Forms.Button calcularPrecios;
        private System.Windows.Forms.CheckBox secureMode;
        private System.Windows.Forms.DataGridViewTextBoxColumn codigo;
        private System.Windows.Forms.DataGridViewTextBoxColumn fAMILIADataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn articulo;
        private System.Windows.Forms.DataGridViewTextBoxColumn compra;
        private System.Windows.Forms.DataGridViewTextBoxColumn iva;
        private System.Windows.Forms.DataGridViewTextBoxColumn margen;
        private System.Windows.Forms.DataGridViewTextBoxColumn pvpCalculado;
        private System.Windows.Forms.DataGridViewTextBoxColumn pvp;
        private System.Windows.Forms.DataGridViewCheckBoxColumn imprimir;
        private System.Windows.Forms.DataGridViewButtonColumn Column1;

    }
}

