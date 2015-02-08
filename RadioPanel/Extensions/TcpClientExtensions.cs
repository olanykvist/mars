
namespace MARS.RadioPanel.Extensions
{
    using System.Linq;
    using System.Net.NetworkInformation;
    using System.Net.Sockets;

    static class TcpClientExtensions
    {
        public static TcpState GetState(this TcpClient client)
        {
            var properties = IPGlobalProperties.GetIPGlobalProperties();
            var info = properties.GetActiveTcpConnections().SingleOrDefault(i => i.LocalEndPoint.Equals(client.Client.LocalEndPoint));

            if (info != null)
            {
                return info.State;
            }
            else
            {
                return TcpState.Unknown;
            }
        }
    }
}
