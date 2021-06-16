namespace autogaz_win
{
	partial class frame
	{
		/// <summary>
		///Gerekli tasarımcı değişkeni.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		///Kullanılan tüm kaynakları temizleyin.
		/// </summary>
		///<param name="disposing">yönetilen kaynaklar dispose edilmeliyse doğru; aksi halde yanlış.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer üretilen kod

		/// <summary>
		/// Tasarımcı desteği için gerekli metot - bu metodun 
		///içeriğini kod düzenleyici ile değiştirmeyin.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frame));
			this.picbox = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.picbox)).BeginInit();
			this.SuspendLayout();
			// 
			// picbox
			// 
			this.picbox.BackColor = System.Drawing.Color.Gray;
			this.picbox.Cursor = System.Windows.Forms.Cursors.Hand;
			this.picbox.Image = global::autogaz_win.Properties.Resources.before;
			this.picbox.ImageLocation = "";
			this.picbox.Location = new System.Drawing.Point(-3, -11);
			this.picbox.Name = "picbox";
			this.picbox.Size = new System.Drawing.Size(700, 507);
			this.picbox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.picbox.TabIndex = 0;
			this.picbox.TabStop = false;
			this.picbox.Click += new System.EventHandler(this.picbox_Click);
			// 
			// frame
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(63)))), ((int)(((byte)(65)))));
			this.ClientSize = new System.Drawing.Size(695, 496);
			this.Controls.Add(this.picbox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "frame";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Autogaz - Windows Edition";
			((System.ComponentModel.ISupportInitialize)(this.picbox)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PictureBox picbox;
	}
}

