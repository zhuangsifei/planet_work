using System;
using System.IO;
using Shell32;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WFMediaPlayer
{
    public partial class MyPlayer : Form
    {
        private int m_TotalSongs = 500;
        private List<string> m_listSongs = null;
        private WMPLib.WindowsMediaPlayer m_myPlayer = null;

        public MyPlayer()
        {
            InitializeComponent();
            m_listSongs = new List<string>();
            m_myPlayer = new WMPLib.WindowsMediaPlayer();
        }

        private void AddFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.InitialDirectory = "G:\\";
            openFileDialog1.Title = "Select songs to play";
            openFileDialog1.Filter = "songs files(*.mp3)|*.mp3; |songs fiels(*.wma)|*.wma";
            openFileDialog1.Multiselect = true;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string[] strSongsPaths = openFileDialog1.FileNames;
                foreach (string songPath in strSongsPaths)
                {
                    if (m_listSongs.Count < m_TotalSongs)
                    {
                        m_listSongs.Add(songPath);
                        this.cmbSongs.Items.Add(songPath);
                    }
                    else
                    {
                        MessageBox.Show(" The list of songs is full!");
                    }
 
                }       
            }
        }

        private void AddFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void btnPrevious_Click(object sender, EventArgs e)
        {
            if (this.cmbSongs.SelectedIndex > 0)
            {
                this.cmbSongs.SelectedIndex--;
            }
            else
            {
                MessageBox.Show("This is already the fisrt song!");
            }
        }

        private void btnPause_Click(object sender, EventArgs e)
        {
            if (m_myPlayer.playState == WMPLib.WMPPlayState.wmppsPaused)
            {
                m_myPlayer.URL = (string)this.cmbSongs.SelectedItem;
                m_myPlayer.controls.play();
            }
            if (m_myPlayer.playState == WMPLib.WMPPlayState.wmppsPlaying)
            {
                m_myPlayer.controls.pause();
            }
 
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            if (cmbSongs.Items.Count >= this.cmbSongs.SelectedIndex + 2)
            {
      
                this.cmbSongs.SelectedIndex++;
            }
            else
            {
                MessageBox.Show("This is the last song");
            }
        }

        private void itemClick_playSongs(object sender,EventArgs e)
        {
            m_myPlayer.URL = (string)this.cmbSongs.SelectedItem;
            m_myPlayer.controls.play();

            // get the during time of the song
            string file = m_myPlayer.URL;
            ShellClass sh = new ShellClass();
            Folder dir = sh.NameSpace(Path.GetDirectoryName(file));
            FolderItem item = dir.ParseName(Path.GetFileName(file));
            string str = dir.GetDetailsOf(item, 27);
            MessageBox.Show(str);
        }

        private void MyPlayer_Load(object sender, EventArgs e)
        {

        }


    }
}
