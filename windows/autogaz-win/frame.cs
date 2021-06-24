using System;
using System.Windows.Forms;
using System.IO;
using System.Media;
using System.Threading;

namespace autogaz_win
{
	public partial class frame : Form
	{

		Thread sndEngineThread;
		int isteGazaGelmekFln = 0;
		SoundPlayer snd;

		public frame()
		{
			InitializeComponent();
			FormClosing += frame_Closing;
			this.MaximizeBox = false;
		}

		private void frame_Closing(Object sender, FormClosingEventArgs e)
		{
			snd.Stop();
		}

		public void sndEngine()
		{
			if (isteGazaGelmekFln == 0)
			{
				isteGazaGelmekFln = 1;
				Control.CheckForIllegalCrossThreadCalls = false;
				picbox.Cursor = Cursors.No;
				Stream str = Properties.Resources.sound;
				snd = new SoundPlayer(str);
				
				snd.Play();

				picbox.Image = Properties.Resources.after;

				Thread.Sleep(7500);

				picbox.Image = Properties.Resources.polat;
				isteGazaGelmekFln = 2;
				picbox.Cursor = Cursors.Hand;

			} else if (isteGazaGelmekFln == 2)
			{
				picbox.Image = Properties.Resources.before;
				isteGazaGelmekFln = 0;
			}
		}

		public void reelfunc()
		{
			if (isteGazaGelmekFln == 1) return;
			sndEngineThread = new Thread(sndEngine);
			sndEngineThread.Start();
		}

		private void picbox_Click(object sender, EventArgs e)
		{
			reelfunc();
		}
	}
}
