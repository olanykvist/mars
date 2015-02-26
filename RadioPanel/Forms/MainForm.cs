
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
        private static Connection connection = new Connection();
        //private Socket socket;

        public MainForm()
        {
            InitializeComponent();
            connection.control = this.mainSatusLabel;
            StartClient();
            this.connectionTimer.Start();
        }

        private void connectionTimer_Tick(object sender, EventArgs e)
        {
            Send(connection, "poke");
        }

        private static void StartClient()
        {
            var endpoint = new IPEndPoint(IPAddress.Loopback, 10112);
            connection.socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            
            //socket.Connect(endpoint);
            connection.socket.BeginConnect(endpoint, new AsyncCallback(OnConnect), connection);
            connectDone.WaitOne();
        }

        private static void OnConnect(IAsyncResult result)
        {
            var socket = (result.AsyncState as Connection).socket;
            var control = (result.AsyncState as Connection).control;

            try
            {
                socket.EndConnect(result);
                control.Text = "Connected!";
                connectDone.Set();
            }
            catch (SocketException exception)
            {
                control.Text = exception.Message;
            }
        }

        //private static void StartClient()
        //{
        //    var endpoint = new IPEndPoint(IPAddress.Loopback, 10112);
        //    var client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        //    connection.client = client;

        //    client.BeginConnect(endpoint, new AsyncCallback(OnConnect), connection);
        //    connectDone.WaitOne();

        //    Send(connection, "Connect");
        //    sendDone.WaitOne();
        //}

        //private static void OnConnect(IAsyncResult result)
        //{
        //    var client = (result.AsyncState as State).client;
        //    var control = (result.AsyncState as State).control;

        //    try
        //    {
        //        client.EndConnect(result);
        //        control.Text = "Connected ok!";
        //        connectDone.Set();
        //    }
        //    catch (SocketException)
        //    {
        //        throw;
        //    }
        //    catch (Exception)
        //    {
        //        throw;
        //    }
        //}

        private static void OnSend(IAsyncResult result)
        {
            var socket = (result.AsyncState as Connection).socket;
            var control = (result.AsyncState as Connection).control;

            try
            {
                var count = socket.EndSend(result);
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

        //private void Send(string message)
        //{
        //    var data = Encoding.ASCII.GetBytes(message);

        //    try
        //    {
        //        socket.Send(data);
        //    }
        //    catch (SocketException)
        //    {
        //        throw;
        //    }
        //}

        private void Send(Connection connection, string message)
        {
            var data = Encoding.ASCII.GetBytes(message);

            try
            {
                //connection.socket.Send(data);
                connection.socket.BeginSend(data, 0, data.Length, 0, new AsyncCallback(OnSend), connection);
            }
            catch (SocketException)
            {
                StartClient();
            }
        }

        private void OnFrequencyChanged(object sender, EventArgs e)
        {
            var radio = sender as Radio;
            var command = new { command = "set", name = "EXT", radio = radio.Id, primary = radio.Frequency, secondary = 0, modulation = radio.Modulation };
            var message = JsonConvert.SerializeObject(command, Formatting.None);
            //Send(message);
            Send(connection, message);
        }
    }

    internal class Connection
    {
        public Socket socket = null;
        public ToolStripItem control = null;
    }
}
