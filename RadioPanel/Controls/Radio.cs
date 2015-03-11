
namespace MARS.RadioPanel.Controls
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Drawing;
    using System.Data;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    using System.Windows.Forms;
    using System.Diagnostics.CodeAnalysis;

    /// <summary>
    /// Defines different modulation types
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.DocumentationRules", "SA1602:EnumerationItemsMustBeDocumented", Justification = "The flag names are self-explanatory.")]
    public enum Modulation
    {
        AM = 0,
        FM = 1
    }

    public partial class Radio : UserControl
    {
        private int id;

        private int frequency;

        private Modulation modulation;

        public Radio()
        {
            this.InitializeComponent();
        }

        public event EventHandler FrequencyChanged;

        public int Id
        {
            get
            {
                return this.id;
            }

            set
            {
                this.id = value;
                this.idLabel.Text = this.id.ToString();
            }
        }

        public int Frequency
        {
            get
            {
                return this.frequency;
            }

            set
            {
                if (value >= 50000 && value <= 399000000)
                {
                    this.frequency = value;
                    this.OnChanged(EventArgs.Empty);
                }
            }
        }

        public Modulation Modulation
        {
            get
            {
                return this.modulation;
            }

            set
            {
                this.modulation = value;
                if (this.modulation == RadioPanel.Controls.Modulation.AM)
                {
                    this.amRadioButton.Checked = true;
                }
                else
                {
                    this.fmRadioButton.Checked = true;
                }
                this.OnChanged(EventArgs.Empty);
            }
        }

        protected virtual void OnChanged(EventArgs e)
        {
            this.frequencyLabel.Text = string.Format("{0:000'.'###,} {1}", this.frequency, this.modulation.ToString());

            if (this.FrequencyChanged != null)
            {
                this.FrequencyChanged(this, e);
            }
        }

        private void increaseMilliButton_Click(object sender, EventArgs e)
        {
            this.Frequency += 1000;
        }

        private void decreaseMilliButton_Click(object sender, EventArgs e)
        {
            this.Frequency -= 1000;
        }

        private void increaseCentiButton_Click(object sender, EventArgs e)
        {
            this.Frequency += 10000;
        }

        private void decreaseCentiButton_Click(object sender, EventArgs e)
        {
            this.Frequency -= 10000;
        }

        private void increaseDeciButton_Click(object sender, EventArgs e)
        {
            this.Frequency += 100000;
        }

        private void decreaseDeciButton_Click(object sender, EventArgs e)
        {
            this.Frequency -= 100000;
        }

        private void increaseButton_Click(object sender, EventArgs e)
        {
            this.Frequency += 1000000;
        }

        private void decreaseButton_Click(object sender, EventArgs e)
        {
            this.Frequency -= 1000000;
        }

        private void increaseDekaButton_Click(object sender, EventArgs e)
        {
            this.Frequency += 10000000;
        }

        private void decreaseDekaButton_Click(object sender, EventArgs e)
        {
            this.Frequency -= 10000000;
        }

        private void increaseHektoButton_Click(object sender, EventArgs e)
        {
            this.Frequency += 100000000;
        }

        private void decreaseHektoButton_Click(object sender, EventArgs e)
        {
            this.Frequency -= 100000000;
        }

        private void amRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            //this.SetModulation();
        }

        private void fmRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            this.SetModulation();
        }

        private void SetModulation()
        {
            if (this.amRadioButton.Checked)
            {
                this.Modulation = RadioPanel.Controls.Modulation.AM;
            }
            else
            {
                this.Modulation = RadioPanel.Controls.Modulation.FM;
            }
        }
    }
}
