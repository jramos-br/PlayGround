using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BlackScreen
{
    public partial class Form1 : Form
    {
        private bool _fullscreen;
        private Rectangle _bounds;

        public Form1()
        {
            InitializeComponent();

            StartPosition = FormStartPosition.CenterScreen;
            Text = nameof(BlackScreen);
            MaximizeBox = false;
            FormBorderStyle = FormBorderStyle.FixedDialog;
            BackColor = Color.Black;
        }

        protected override void OnLoad(EventArgs e)
        {
            SetFullscreenMode(true);
            base.OnLoad(e);
        }

        protected override void OnKeyPress(System.Windows.Forms.KeyPressEventArgs e)
        {
            switch (e.KeyChar)
            {
                case (Char)Keys.Escape:
                    Close();
                    e.Handled = true;
                    break;
                case '-':
                    if (WindowState == FormWindowState.Normal)
                        WindowState = FormWindowState.Minimized;
                    break;
                default:
                    if (WindowState == FormWindowState.Normal)
                        SetFullscreenMode(!_fullscreen);
                    break;
            }
        }

        private void SetFullscreenMode(bool fullscreen)
        {
            if (fullscreen != _fullscreen)
            {
                if (fullscreen)
                {
                    _bounds = Bounds;
                    FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                    Bounds = Screen.FromControl(this).Bounds;
                }
                else
                {
                    FormBorderStyle = FormBorderStyle.FixedDialog;
                    Bounds = _bounds;
                }

                _fullscreen = fullscreen;
            }
        }
    }
}
