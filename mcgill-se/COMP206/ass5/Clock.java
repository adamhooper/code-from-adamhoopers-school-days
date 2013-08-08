import javax.swing.JApplet;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class Clock extends JApplet
{
	private Color bgColor = new Color(0xd7, 0xbc, 0x7a);
	public Timer timer;

	public void start()
	{
		TimerTask task = new TimerTask() {
			public void run()
			{
				repaint();
			}
		};

		timer = new Timer();
		timer.schedule(task, 250, 250);
	}

	public void stop()
	{
		timer.cancel();
		timer = null;
	}

	public void paint(Graphics g)
	{
		Date date = new Date();

		Dimension d = getSize();

		g.setColor(bgColor);
		g.fillRect(0, 0, d.width, d.height);

		g.setColor(Color.BLACK);
		g.drawString("" + date, 10, 20);
	}
}
