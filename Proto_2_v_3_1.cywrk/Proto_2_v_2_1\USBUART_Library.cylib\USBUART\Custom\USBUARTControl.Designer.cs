namespace USBUART
{
    partial class USBUARTControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBoxParams = new System.Windows.Forms.GroupBox();
            this.textBoxProductString = new System.Windows.Forms.TextBox();
            this.textBoxManufacturerString = new System.Windows.Forms.TextBox();
            this.numUpDownRelease = new System.Windows.Forms.NumericUpDown();
            this.numUpDownPID = new System.Windows.Forms.NumericUpDown();
            this.numUpDownVID = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label0x = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBoxParams.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownRelease)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownPID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownVID)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBoxParams
            // 
            this.groupBoxParams.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxParams.Controls.Add(this.textBoxProductString);
            this.groupBoxParams.Controls.Add(this.textBoxManufacturerString);
            this.groupBoxParams.Controls.Add(this.numUpDownRelease);
            this.groupBoxParams.Controls.Add(this.numUpDownPID);
            this.groupBoxParams.Controls.Add(this.numUpDownVID);
            this.groupBoxParams.Controls.Add(this.label10);
            this.groupBoxParams.Controls.Add(this.label9);
            this.groupBoxParams.Controls.Add(this.label0x);
            this.groupBoxParams.Controls.Add(this.label8);
            this.groupBoxParams.Controls.Add(this.label7);
            this.groupBoxParams.Controls.Add(this.label3);
            this.groupBoxParams.Controls.Add(this.label2);
            this.groupBoxParams.Controls.Add(this.label1);
            this.groupBoxParams.Location = new System.Drawing.Point(3, 2);
            this.groupBoxParams.Name = "groupBoxParams";
            this.groupBoxParams.Size = new System.Drawing.Size(248, 154);
            this.groupBoxParams.TabIndex = 1;
            this.groupBoxParams.TabStop = false;
            this.groupBoxParams.Text = "Device Attributes";
            // 
            // textBoxProductString
            // 
            this.textBoxProductString.Location = new System.Drawing.Point(124, 123);
            this.textBoxProductString.Name = "textBoxProductString";
            this.textBoxProductString.Size = new System.Drawing.Size(109, 20);
            this.textBoxProductString.TabIndex = 19;
            this.textBoxProductString.TextChanged += new System.EventHandler(this.textBoxProductString_TextChanged);
            // 
            // textBoxManufacturerString
            // 
            this.textBoxManufacturerString.Location = new System.Drawing.Point(124, 96);
            this.textBoxManufacturerString.Name = "textBoxManufacturerString";
            this.textBoxManufacturerString.Size = new System.Drawing.Size(109, 20);
            this.textBoxManufacturerString.TabIndex = 18;
            this.textBoxManufacturerString.TextChanged += new System.EventHandler(this.textBoxManufacturerString_TextChanged);
            // 
            // numUpDownRelease
            // 
            this.numUpDownRelease.Hexadecimal = true;
            this.numUpDownRelease.Location = new System.Drawing.Point(124, 69);
            this.numUpDownRelease.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numUpDownRelease.Name = "numUpDownRelease";
            this.numUpDownRelease.Size = new System.Drawing.Size(109, 20);
            this.numUpDownRelease.TabIndex = 3;
            this.numUpDownRelease.ValueChanged += new System.EventHandler(this.numUpDownRelease_ValueChanged);
            // 
            // numUpDownPID
            // 
            this.numUpDownPID.Hexadecimal = true;
            this.numUpDownPID.Location = new System.Drawing.Point(124, 42);
            this.numUpDownPID.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numUpDownPID.Name = "numUpDownPID";
            this.numUpDownPID.Size = new System.Drawing.Size(109, 20);
            this.numUpDownPID.TabIndex = 2;
            this.numUpDownPID.ValueChanged += new System.EventHandler(this.numUpDownPID_ValueChanged);
            // 
            // numUpDownVID
            // 
            this.numUpDownVID.Hexadecimal = true;
            this.numUpDownVID.Location = new System.Drawing.Point(124, 14);
            this.numUpDownVID.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numUpDownVID.Name = "numUpDownVID";
            this.numUpDownVID.Size = new System.Drawing.Size(109, 20);
            this.numUpDownVID.TabIndex = 1;
            this.numUpDownVID.ValueChanged += new System.EventHandler(this.numUpDownVID_ValueChanged);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(105, 71);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(18, 13);
            this.label10.TabIndex = 17;
            this.label10.Text = "0x";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(105, 44);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(18, 13);
            this.label9.TabIndex = 16;
            this.label9.Text = "0x";
            // 
            // label0x
            // 
            this.label0x.AutoSize = true;
            this.label0x.Location = new System.Drawing.Point(105, 16);
            this.label0x.Name = "label0x";
            this.label0x.Size = new System.Drawing.Size(18, 13);
            this.label0x.TabIndex = 5;
            this.label0x.Text = "0x";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(7, 71);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(83, 13);
            this.label8.TabIndex = 14;
            this.label8.Text = "Device Release";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(4, 126);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(74, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Product String";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(100, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Manufacturer String";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Product ID";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Vendor ID";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 168);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(293, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "The .inf file can be found in     ...\\Generated_Source\\PSoC3";
            // 
            // USBUARTControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.groupBoxParams);
            this.Name = "USBUARTControl";
            this.Size = new System.Drawing.Size(482, 242);
            this.groupBoxParams.ResumeLayout(false);
            this.groupBoxParams.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownRelease)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownPID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownVID)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxParams;
        public System.Windows.Forms.NumericUpDown numUpDownRelease;
        public System.Windows.Forms.NumericUpDown numUpDownPID;
        public System.Windows.Forms.NumericUpDown numUpDownVID;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label0x;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.TextBox textBoxProductString;
        public System.Windows.Forms.TextBox textBoxManufacturerString;
        private System.Windows.Forms.Label label4;
    }
}
