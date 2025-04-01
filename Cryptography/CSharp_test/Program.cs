using System;
using System.Windows.Forms;

namespace RandomNumberGenerator
{
    public partial class Form1 : Form
    {
        private Random random;

        public Form1()
        {
            InitializeComponent();
            random = new Random();
        }

        private void btnGenerate_Click(object sender, EventArgs e)
        {
            int a, b;

            // Validate input
            if (int.TryParse(txtA.Text, out a) && int.TryParse(txtB.Text, out b))
            {
                if (a < b)
                {
                    int randomNumber = random.Next(a, b + 1); // Generate the random number
                    lblResult.Text = $"Random Number: {randomNumber}";
                }
                else
                {
                    MessageBox.Show("Please ensure that 'a' is less than 'b'.", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Please enter valid integers for both 'a' and 'b'.", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}