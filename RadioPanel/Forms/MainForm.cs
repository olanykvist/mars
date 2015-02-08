
namespace MARS.RadioPanel.Forms
{
    using MARS.RadioPanel.Controls;
    using Newtonsoft.Json;
    using System;
    using System.Net;
    using System.Net.Sockets;
    using System.Text;
    using System.Threading;
    using System.Windows.Forms;

    public partial class MainForm : Form
    {
        private static ManualResetEvent connectDone = new ManualResetEvent(false);
        private static ManualResetEvent sendDone = new ManualResetEvent(false);
        private static State state = new State();

        public MainForm()
        {
            InitializeComponent();
            this.StartClient();
            this.connectionTimer.Start();
        }

        private void connectionTimer_Tick(object sender, EventArgs e)
        {
            Send(state, "ping");
        }

        private void StartClient()
        {
            var endpoint = new IPEndPoint(IPAddress.Loopback, 2000);
            var client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            state.client = client;
            state.control = this.mainSatusLabel;

            client.BeginConnect(endpoint, new AsyncCallback(OnConnect), state);
            connectDone.WaitOne();

            Send(state, "Connect");
            sendDone.WaitOne();
        }

        private static void OnConnect(IAsyncResult result)
        {
            var client = (result.AsyncState as State).client;
            var control = (result.AsyncState as State).control;

            try
            {
                client.EndConnect(result);
                control.Text = "Connected ok!";
                connectDone.Set();
            }
            catch (SocketException)
            {
                throw;
            }
            catch (Exception)
            {
                throw;
            }
        }

        private static void OnSend(IAsyncResult result)
        {
            var client = (result.AsyncState as State).client;
            var control = (result.AsyncState as State).control;

            try
            {
                var count = client.EndSend(result);
                control.Text = "Sent ok!";
                sendDone.Set();
            }
            catch (SocketException)
            {
                control.Text = "Send failed";
                throw;
            }
            catch (Exception)
            {
                throw;
            }
        }

        private static void Send(State state, string message)
        {
            var data = Encoding.ASCII.GetBytes(message);
            state.client.BeginSend(data, 0, data.Length, 0, new AsyncCallback(OnSend), state);
        }

        private void OnFrequencyChanged(object sender, EventArgs e)
        {
            var radio = sender as Radio;
            var command = new { Command = "setext", Radio = radio.Id, Frequency = radio.Frequency, Modulation = radio.Modulation };
            var message = JsonConvert.SerializeObject(command, Formatting.None);
            Send(state, message);
        }
    }
}
