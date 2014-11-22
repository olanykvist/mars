namespace MARS.RadioPanel.Controls
{
    partial class Radio
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
            System.Windows.Forms.Label idCaption;
            this.frequencyLabel = new System.Windows.Forms.Label();
            this.idLabel = new System.Windows.Forms.Label();
            this.amRadioButton = new System.Windows.Forms.RadioButton();
            this.fmRadioButton = new System.Windows.Forms.RadioButton();
            this.decreaseDeciButton = new System.Windows.Forms.Button();
            this.decreaseCentiButton = new System.Windows.Forms.Button();
            this.decreaseMilliButton = new System.Windows.Forms.Button();
            this.decreaseButton = new System.Windows.Forms.Button();
            this.decreaseDekaButton = new System.Windows.Forms.Button();
            this.decreaseHektoButton = new System.Windows.Forms.Button();
            this.increaseButton = new System.Windows.Forms.Button();
            this.increaseDekaButton = new System.Windows.Forms.Button();
            this.increaseHektoButton = new System.Windows.Forms.Button();
            this.increaseMilliButton = new System.Windows.Forms.Button();
            this.increaseCentiButton = new System.Windows.Forms.Button();
            this.increaseDeciButton = new System.Windows.Forms.Button();
            idCaption = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // frequencyLabel
            // 
            this.frequencyLabel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.frequencyLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.frequencyLabel.Font = new System.Drawing.Font("Courier New", 15F, System.Drawing.FontStyle.Bold);
            this.frequencyLabel.ForeColor = System.Drawing.Color.Lime;
            this.frequencyLabel.Location = new System.Drawing.Point(7, 42);
            this.frequencyLabel.Name = "frequencyLabel";
            this.frequencyLabel.Size = new System.Drawing.Size(148, 30);
            this.frequencyLabel.TabIndex = 0;
            this.frequencyLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // idLabel
            // 
            this.idLabel.AutoSize = true;
            this.idLabel.Location = new System.Drawing.Point(40, 3);
            this.idLabel.Name = "idLabel";
            this.idLabel.Size = new System.Drawing.Size(14, 13);
            this.idLabel.TabIndex = 1;
            this.idLabel.Text = "X";
            // 
            // amRadioButton
            // 
            this.amRadioButton.AutoSize = true;
            this.amRadioButton.Location = new System.Drawing.Point(79, 1);
            this.amRadioButton.Name = "amRadioButton";
            this.amRadioButton.Size = new System.Drawing.Size(41, 17);
            this.amRadioButton.TabIndex = 3;
            this.amRadioButton.TabStop = true;
            this.amRadioButton.Text = "AM";
            this.amRadioButton.UseVisualStyleBackColor = true;
            this.amRadioButton.CheckedChanged += new System.EventHandler(this.amRadioButton_CheckedChanged);
            // 
            // fmRadioButton
            // 
            this.fmRadioButton.AutoSize = true;
            this.fmRadioButton.Location = new System.Drawing.Point(119, 1);
            this.fmRadioButton.Name = "fmRadioButton";
            this.fmRadioButton.Size = new System.Drawing.Size(40, 17);
            this.fmRadioButton.TabIndex = 4;
            this.fmRadioButton.TabStop = true;
            this.fmRadioButton.Text = "FM";
            this.fmRadioButton.UseVisualStyleBackColor = true;
            this.fmRadioButton.CheckedChanged += new System.EventHandler(this.fmRadioButton_CheckedChanged);
            // 
            // idCaption
            // 
            idCaption.AutoSize = true;
            idCaption.Location = new System.Drawing.Point(3, 3);
            idCaption.Name = "idCaption";
            idCaption.Size = new System.Drawing.Size(38, 13);
            idCaption.TabIndex = 5;
            idCaption.Text = "Radio:";
            // 
            // decreaseDeciButton
            // 
            this.decreaseDeciButton.Location = new System.Drawing.Point(72, 76);
            this.decreaseDeciButton.Name = "decreaseDeciButton";
            this.decreaseDeciButton.Size = new System.Drawing.Size(16, 16);
            this.decreaseDeciButton.TabIndex = 6;
            this.decreaseDeciButton.UseVisualStyleBackColor = true;
            this.decreaseDeciButton.Click += new System.EventHandler(this.decreaseDeciButton_Click);
            // 
            // decreaseCentiButton
            // 
            this.decreaseCentiButton.Location = new System.Drawing.Point(87, 76);
            this.decreaseCentiButton.Name = "decreaseCentiButton";
            this.decreaseCentiButton.Size = new System.Drawing.Size(16, 16);
            this.decreaseCentiButton.TabIndex = 7;
            this.decreaseCentiButton.UseVisualStyleBackColor = true;
            this.decreaseCentiButton.Click += new System.EventHandler(this.decreaseCentiButton_Click);
            // 
            // decreaseMilliButton
            // 
            this.decreaseMilliButton.Location = new System.Drawing.Point(102, 76);
            this.decreaseMilliButton.Name = "decreaseMilliButton";
            this.decreaseMilliButton.Size = new System.Drawing.Size(16, 16);
            this.decreaseMilliButton.TabIndex = 8;
            this.decreaseMilliButton.UseVisualStyleBackColor = true;
            this.decreaseMilliButton.Click += new System.EventHandler(this.decreaseMilliButton_Click);
            // 
            // decreaseButton
            // 
            this.decreaseButton.Location = new System.Drawing.Point(53, 76);
            this.decreaseButton.Name = "decreaseButton";
            this.decreaseButton.Size = new System.Drawing.Size(16, 16);
            this.decreaseButton.TabIndex = 11;
            this.decreaseButton.UseVisualStyleBackColor = true;
            this.decreaseButton.Click += new System.EventHandler(this.decreaseButton_Click);
            // 
            // decreaseDekaButton
            // 
            this.decreaseDekaButton.Location = new System.Drawing.Point(38, 76);
            this.decreaseDekaButton.Name = "decreaseDekaButton";
            this.decreaseDekaButton.Size = new System.Drawing.Size(16, 16);
            this.decreaseDekaButton.TabIndex = 10;
            this.decreaseDekaButton.UseVisualStyleBackColor = true;
            this.decreaseDekaButton.Click += new System.EventHandler(this.decreaseDekaButton_Click);
            // 
            // decreaseHektoButton
            // 
            this.decreaseHektoButton.Location = new System.Drawing.Point(23, 76);
            this.decreaseHektoButton.Name = "decreaseHektoButton";
            this.decreaseHektoButton.Size = new System.Drawing.Size(16, 16);
            this.decreaseHektoButton.TabIndex = 9;
            this.decreaseHektoButton.UseVisualStyleBackColor = true;
            this.decreaseHektoButton.Click += new System.EventHandler(this.decreaseHektoButton_Click);
            // 
            // increaseButton
            // 
            this.increaseButton.Location = new System.Drawing.Point(53, 23);
            this.increaseButton.Name = "increaseButton";
            this.increaseButton.Size = new System.Drawing.Size(16, 16);
            this.increaseButton.TabIndex = 17;
            this.increaseButton.UseVisualStyleBackColor = true;
            this.increaseButton.Click += new System.EventHandler(this.increaseButton_Click);
            // 
            // increaseDekaButton
            // 
            this.increaseDekaButton.Location = new System.Drawing.Point(38, 23);
            this.increaseDekaButton.Name = "increaseDekaButton";
            this.increaseDekaButton.Size = new System.Drawing.Size(16, 16);
            this.increaseDekaButton.TabIndex = 16;
            this.increaseDekaButton.UseVisualStyleBackColor = true;
            this.increaseDekaButton.Click += new System.EventHandler(this.increaseDekaButton_Click);
            // 
            // increaseHektoButton
            // 
            this.increaseHektoButton.Location = new System.Drawing.Point(23, 23);
            this.increaseHektoButton.Name = "increaseHektoButton";
            this.increaseHektoButton.Size = new System.Drawing.Size(16, 16);
            this.increaseHektoButton.TabIndex = 15;
            this.increaseHektoButton.UseVisualStyleBackColor = true;
            this.increaseHektoButton.Click += new System.EventHandler(this.increaseHektoButton_Click);
            // 
            // increaseMilliButton
            // 
            this.increaseMilliButton.Location = new System.Drawing.Point(102, 23);
            this.increaseMilliButton.Name = "increaseMilliButton";
            this.increaseMilliButton.Size = new System.Drawing.Size(16, 16);
            this.increaseMilliButton.TabIndex = 14;
            this.increaseMilliButton.UseVisualStyleBackColor = true;
            this.increaseMilliButton.Click += new System.EventHandler(this.increaseMilliButton_Click);
            // 
            // increaseCentiButton
            // 
            this.increaseCentiButton.Location = new System.Drawing.Point(87, 23);
            this.increaseCentiButton.Name = "increaseCentiButton";
            this.increaseCentiButton.Size = new System.Drawing.Size(16, 16);
            this.increaseCentiButton.TabIndex = 13;
            this.increaseCentiButton.UseVisualStyleBackColor = true;
            this.increaseCentiButton.Click += new System.EventHandler(this.increaseCentiButton_Click);
            // 
            // increaseDeciButton
            // 
            this.increaseDeciButton.Location = new System.Drawing.Point(72, 23);
            this.increaseDeciButton.Name = "increaseDeciButton";
            this.increaseDeciButton.Size = new System.Drawing.Size(16, 16);
            this.increaseDeciButton.TabIndex = 12;
            this.increaseDeciButton.UseVisualStyleBackColor = true;
            this.increaseDeciButton.Click += new System.EventHandler(this.increaseDeciButton_Click);
            // 
            // Radio
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.Controls.Add(this.increaseButton);
            this.Controls.Add(this.increaseDekaButton);
            this.Controls.Add(this.increaseHektoButton);
            this.Controls.Add(this.increaseMilliButton);
            this.Controls.Add(this.increaseCentiButton);
            this.Controls.Add(this.increaseDeciButton);
            this.Controls.Add(this.decreaseButton);
            this.Controls.Add(this.decreaseDekaButton);
            this.Controls.Add(this.decreaseHektoButton);
            this.Controls.Add(this.decreaseMilliButton);
            this.Controls.Add(this.decreaseCentiButton);
            this.Controls.Add(this.decreaseDeciButton);
            this.Controls.Add(idCaption);
            this.Controls.Add(this.fmRadioButton);
            this.Controls.Add(this.amRadioButton);
            this.Controls.Add(this.idLabel);
            this.Controls.Add(this.frequencyLabel);
            this.Name = "Radio";
            this.Size = new System.Drawing.Size(163, 95);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label frequencyLabel;
        private System.Windows.Forms.Label idLabel;
        private System.Windows.Forms.RadioButton amRadioButton;
        private System.Windows.Forms.RadioButton fmRadioButton;
        private System.Windows.Forms.Button decreaseDeciButton;
        private System.Windows.Forms.Button decreaseCentiButton;
        private System.Windows.Forms.Button decreaseMilliButton;
        private System.Windows.Forms.Button decreaseButton;
        private System.Windows.Forms.Button decreaseDekaButton;
        private System.Windows.Forms.Button decreaseHektoButton;
        private System.Windows.Forms.Button increaseButton;
        private System.Windows.Forms.Button increaseDekaButton;
        private System.Windows.Forms.Button increaseHektoButton;
        private System.Windows.Forms.Button increaseMilliButton;
        private System.Windows.Forms.Button increaseCentiButton;
        private System.Windows.Forms.Button increaseDeciButton;
    }
}
