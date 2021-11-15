using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;


namespace client_ui
{
    public partial class MainWindow : Window
    {
        System.Net.Sockets.TcpClient clientSocket = new System.Net.Sockets.TcpClient();
        NetworkStream serverStream = default(NetworkStream);

        public MainWindow()
        {
            InitializeComponent();
        }

        public void ConnectButtonClick(object sender, EventArgs e)
        {
            clientSocket.Connect("127.0.0.1", 27015);

            TextBlock statusTextBlock = (TextBlock)FindName("statusText");
            TextBlock usernameTextBlock = (TextBlock)FindName("userNameText");
            TextBox userNameTextBox = (TextBox)FindName("userNameTextBox");
            statusTextBlock.Text = statusTextBlock.Text + "connected";
            userNameText.Text = userNameText.Text + " " + userNameTextBox.Text;
        }


    }
}
