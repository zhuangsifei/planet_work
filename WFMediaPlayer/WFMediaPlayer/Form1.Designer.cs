namespace WFMediaPlayer
{
    partial class MyPlayer
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MyPlayer));
            this.gbButtons = new System.Windows.Forms.GroupBox();
            this.btnNext = new System.Windows.Forms.Button();
            this.btnPause = new System.Windows.Forms.Button();
            this.btnPrevious = new System.Windows.Forms.Button();
            this.prgBarMusic = new System.Windows.Forms.ProgressBar();
            this.ctMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.AddFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.AddFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cmbSongs = new System.Windows.Forms.ComboBox();
            this.gbButtons.SuspendLayout();
            this.ctMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbButtons
            // 
            this.gbButtons.BackColor = System.Drawing.SystemColors.Control;
            this.gbButtons.CausesValidation = false;
            this.gbButtons.Controls.Add(this.btnNext);
            this.gbButtons.Controls.Add(this.btnPause);
            this.gbButtons.Controls.Add(this.btnPrevious);
            this.gbButtons.Controls.Add(this.prgBarMusic);
            this.gbButtons.ForeColor = System.Drawing.SystemColors.ButtonFace;
            this.gbButtons.Location = new System.Drawing.Point(12, 12);
            this.gbButtons.Name = "gbButtons";
            this.gbButtons.Size = new System.Drawing.Size(346, 108);
            this.gbButtons.TabIndex = 0;
            this.gbButtons.TabStop = false;
            // 
            // btnNext
            // 
            this.btnNext.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnNext.BackgroundImage")));
            this.btnNext.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btnNext.CausesValidation = false;
            this.btnNext.Location = new System.Drawing.Point(175, 37);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(45, 45);
            this.btnNext.TabIndex = 3;
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // btnPause
            // 
            this.btnPause.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnPause.BackgroundImage")));
            this.btnPause.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btnPause.Location = new System.Drawing.Point(111, 37);
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(45, 45);
            this.btnPause.TabIndex = 2;
            this.btnPause.UseVisualStyleBackColor = true;
            this.btnPause.Click += new System.EventHandler(this.btnPause_Click);
            // 
            // btnPrevious
            // 
            this.btnPrevious.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btnPrevious.BackgroundImage")));
            this.btnPrevious.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btnPrevious.Location = new System.Drawing.Point(49, 37);
            this.btnPrevious.Name = "btnPrevious";
            this.btnPrevious.Size = new System.Drawing.Size(45, 45);
            this.btnPrevious.TabIndex = 1;
            this.btnPrevious.UseVisualStyleBackColor = true;
            this.btnPrevious.Click += new System.EventHandler(this.btnPrevious_Click);
            // 
            // prgBarMusic
            // 
            this.prgBarMusic.Location = new System.Drawing.Point(7, 21);
            this.prgBarMusic.Name = "prgBarMusic";
            this.prgBarMusic.Size = new System.Drawing.Size(333, 5);
            this.prgBarMusic.TabIndex = 0;
            // 
            // ctMenu
            // 
            this.ctMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.AddFileToolStripMenuItem,
            this.AddFolderToolStripMenuItem});
            this.ctMenu.Name = "contextMenuStrip1";
            this.ctMenu.Size = new System.Drawing.Size(159, 48);
            // 
            // AddFileToolStripMenuItem
            // 
            this.AddFileToolStripMenuItem.Name = "AddFileToolStripMenuItem";
            this.AddFileToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.AddFileToolStripMenuItem.Text = "添加歌曲文件";
            this.AddFileToolStripMenuItem.Click += new System.EventHandler(this.AddFileToolStripMenuItem_Click);
            // 
            // AddFolderToolStripMenuItem
            // 
            this.AddFolderToolStripMenuItem.Name = "AddFolderToolStripMenuItem";
            this.AddFolderToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.AddFolderToolStripMenuItem.Text = "添加歌曲文件夹";
            this.AddFolderToolStripMenuItem.Click += new System.EventHandler(this.AddFolderToolStripMenuItem_Click);
            // 
            // cmbSongs
            // 
            this.cmbSongs.AllowDrop = true;
            this.cmbSongs.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.cmbSongs.ContextMenuStrip = this.ctMenu;
            this.cmbSongs.FormattingEnabled = true;
            this.cmbSongs.Location = new System.Drawing.Point(12, 142);
            this.cmbSongs.Name = "cmbSongs";
            this.cmbSongs.Size = new System.Drawing.Size(340, 20);
            this.cmbSongs.TabIndex = 3;
            this.cmbSongs.Text = "默认歌曲列表";
            this.cmbSongs.SelectedIndexChanged += new System.EventHandler(this.itemClick_playSongs);
            // 
            // MyPlayer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(364, 421);
            this.Controls.Add(this.cmbSongs);
            this.Controls.Add(this.gbButtons);
            this.Name = "MyPlayer";
            this.Text = "You Don\'t known";
            this.Load += new System.EventHandler(this.MyPlayer_Load);
            this.gbButtons.ResumeLayout(false);
            this.ctMenu.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbButtons;
        private System.Windows.Forms.ProgressBar prgBarMusic;
        private System.Windows.Forms.Button btnPrevious;
        private System.Windows.Forms.Button btnPause;
        private System.Windows.Forms.Button btnNext;
        private System.Windows.Forms.ContextMenuStrip ctMenu;
        private System.Windows.Forms.ComboBox cmbSongs;
        private System.Windows.Forms.ToolStripMenuItem AddFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem AddFolderToolStripMenuItem;
    }
}

