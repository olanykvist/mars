
namespace MARS.RadioPanel.Forms
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Data;
    using System.Drawing;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    using System.Windows.Forms;
    using System.Net;
    using System.Net.Sockets;
    using System.IO;

    public partial class MainForm : Form
    {
        private TcpClient client;
        private NetworkStream stream;

        public MainForm()
        {
            InitializeComponent();

            this.client = new TcpClient();
            this.client.NoDelay = true;

            this.connectionTimer.Start();
        }

        private void Send(string message)
        {
            if (this.client.Connected)
            {
                // Send
                try
                {
                    var data = Encoding.ASCII.GetBytes(message);
                    this.stream = this.client.GetStream();
                    stream.Write(data, 0, data.Length);
                    this.mainSatusLabel.Text = "Status: Connected!";
                }
                catch (IOException)
                {
                    this.mainSatusLabel.Text = "Status: Failed to send!";
                }
            }
            else
            {
                try
                {
                    this.client.Connect(IPAddress.Loopback, 2000);
                    this.Send(message);
                }
                catch (SocketException)
                {
                    this.mainSatusLabel.Text = "Status: Failed to connect!";
                }
            }
        }

        private void connectionTimer_Tick(object sender, EventArgs e)
        {
            this.Send("Hello!");
        }
    }
}
