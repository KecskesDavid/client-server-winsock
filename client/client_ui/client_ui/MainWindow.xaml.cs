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

public enum RequestType
{
    CREATE_SOCKET = 0,
	CREATE_GROUP = 1,
	SEND_PUBLIC = 2,
	SEND_GROUP = 3,
	SEND_PRIVATE = 4,
    ADD_TO_GROUP = 5,
}

namespace client_ui
{
    public partial class MainWindow : Window
    {
        private System.Net.Sockets.TcpClient _clientSocket = new System.Net.Sockets.TcpClient();
        private NetworkStream _serverStream = default(NetworkStream);
        private string _username;

        public MainWindow()
        {
            InitializeComponent();
        }

        public void ConnectButtonClick(object sender, EventArgs e)
        {
            // connect to the server with the specific address
            _clientSocket.Connect("127.0.0.1", 27015);
            // setup every ui change that should be done
            setupConnectionChangesOnUI();
            // Login, create socket
            login();
        }

        private void SendButtonClick(object sender, RoutedEventArgs e)
        {
            NetworkStream serverStream = _clientSocket.GetStream();

            byte[] outStream = System.Text.Encoding.ASCII.GetBytes("Valami");
            serverStream.Write(outStream, 0, outStream.Length);
            serverStream.Flush();
        }

        private void SelectionChangedHandler(object sender, SelectionChangedEventArgs e)
        {
            ComboBox messageTypeSelector = (ComboBox)sender;
            ComboBoxItem selectedType = (ComboBoxItem)messageTypeSelector.SelectedItem;

            setupSenderText(selectedType.Name);
        }

        /// First request after connection, this should be done for iniatilization porpoises
        private void login()
        {
            NetworkStream serverStream = _clientSocket.GetStream();
            TextBox userNameTextBox = (TextBox)FindName("userNameTextBox");

            String outStreamString = createOutStream(RequestType.CREATE_SOCKET, _username, "", "");
            byte[] outStream = System.Text.Encoding.ASCII.GetBytes(outStreamString);

            serverStream.Write(outStream, 0, outStream.Length);
            serverStream.Flush();
        }

        /// Ui changes after the connection to the server
        private void setupConnectionChangesOnUI()
        {
            // TODO make everything unclickable when not connected, then setup
            TextBlock statusTextBlock = (TextBlock)FindName("statusText");
            TextBox userNameTextBox = (TextBox)FindName("userNameTextBox");
            _username = userNameTextBox.Text;
            statusTextBlock.Text = statusTextBlock.Text + " connected";
            userNameText.Text = userNameText.Text + " " + userNameTextBox.Text;
        }

        /// Makes the sender text box visible/hidden 
        private void setupSenderText(string selectedItem)
        {
            TextBox senderName = (TextBox)FindName("senderNameText");
            if (selectedItem == "Private" || selectedItem == "Group")
            {
                senderName.Visibility = Visibility.Visible;
            } else
            {
                senderName.Visibility = Visibility.Hidden;
            }
        }

        private String createOutStream(RequestType requestType, string sender, string receiver, string message)
        {
            return new StringBuilder()
                .Append((int)requestType) // Firstly the type of request
                .Append("|")
                .Append(sender) // Then the sender
                .Append("|")
                .Append(receiver) // Then the receiver(s)
                .Append("|")
                .Append(message)
                .ToString(); // Then the message itself
        }
    }
}
