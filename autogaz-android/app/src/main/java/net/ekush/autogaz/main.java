package net.ekush.autogaz;

import androidx.appcompat.app.AppCompatActivity;

import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
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
        final Button button = (Button) findViewById(R.id.button);
        final ImageView imgPanel = (ImageView) findViewById(R.id.imagePanel);

        if (state == 0) {

            imgPanel.setImageResource(R.drawable.after);
            button.setText("Gaza Geliyorsun!");
            button.setEnabled(false);
            mp.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {

                @Override
                public void onCompletion(MediaPlayer mp) {
                    state = 1;
                    imgPanel.setImageResource(R.drawable.polat);
                    button.setText("eyv abi");
                    button.setEnabled(true);
                }

            });

            mp.start();

        } else if (state == 1) {

            imgPanel.setImageResource(R.drawable.before);
            state = 0;
            button.setText("Gaza Gel!");
            button.setEnabled(true);
        }

        //Toast.makeText(main.this, "This is my Toast message!", Toast.LENGTH_LONG).show();
    }
}