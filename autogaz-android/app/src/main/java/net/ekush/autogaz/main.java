package net.ekush.autogaz;

import androidx.appcompat.app.AppCompatActivity;

import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class main extends AppCompatActivity {


    public int state = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    public void onClickBtn(View v)
    {
        final MediaPlayer mp = MediaPlayer.create(main.this, R.raw.music);
        final ImageView imgPanel = (ImageView) findViewById(R.id.imagePanel);

        if (state == 0) {

            imgPanel.setImageResource(R.drawable.after);
            mp.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {

                @Override
                public void onCompletion(MediaPlayer mp) {
                    state = 1;
                    imgPanel.setImageResource(R.drawable.polat);
                }

            });

            mp.start();

        } else if (state == 1) {

            imgPanel.setImageResource(R.drawable.before);
            state = 0;
        }

    }
}