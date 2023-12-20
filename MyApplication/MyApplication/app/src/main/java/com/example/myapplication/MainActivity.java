package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

import org.json.JSONArray;
import org.json.JSONObject;

import java.net.URL;

import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {

    private static final String URL_WRITE = "https://api.thingspeak.com/update?api_key=MAWOVDXG0QX5X9XG";
    private static final String URL_READ = "https://api.thingspeak.com/channels/2379033/fields/2.json?results=10";

    private TextView txt;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        txt = findViewById(R.id.main_txt);
        readJSON();
    }

    public void readJSON(){
        AsyncHttpClient client = new AsyncHttpClient();
        client.get(URL_READ, new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                if (statusCode==200){
                    String response = new String(responseBody);
                    processJSON(response);
                }

            }
            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {

            }
        });
    }
    private void processJSON(String json){
        try{
            JSONObject root = new JSONObject(json);
            JSONArray feeds = root.getJSONArray("feeds");
            String valor, texto="";

            for(int i = 0; i<feeds.length(); i++){
                valor = feeds.getJSONObject(i).getString("field2");
                texto = texto + valor + "\n";
            }
            txt.setText(texto);

        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void alarmaOn(View view) {
        final int[] statusCode = new int[1];
        do {
            RequestParams params = new RequestParams("field3", "1");

            AsyncHttpClient client = new AsyncHttpClient();
            client.get(URL_WRITE, params, new AsyncHttpResponseHandler() {
                @Override
                public void onSuccess(int code, Header[] headers, byte[] responseBody) {
                    statusCode[0] = code;
                    if (statusCode[0] == 200) {
                        String response = new String(responseBody);
                        Toast.makeText(MainActivity.this, response, Toast.LENGTH_SHORT).show();
                    }
                }

                @Override
                public void onFailure(int code, Header[] headers, byte[] responseBody, Throwable error) {
                    // Manejar el fallo de la solicitud aquí si es necesario
                    statusCode[0] = code;
                }
            });

            try {
                // Agregar un retardo de 2.5 segundos
                Thread.sleep(2500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } while (statusCode[0] == 0); // Repite mientras el código de estado sea 0
    }

    public void alarmaoff(View view) {
        final int[] statusCode = new int[1];
        do {
            RequestParams params = new RequestParams("field3", "0");

            AsyncHttpClient client = new AsyncHttpClient();
            client.get(URL_WRITE, params, new AsyncHttpResponseHandler() {
                @Override
                public void onSuccess(int code, Header[] headers, byte[] responseBody) {
                    statusCode[0] = code;
                    if (statusCode[0] == 200) {
                        String response = new String(responseBody);
                        Toast.makeText(MainActivity.this, response, Toast.LENGTH_SHORT).show();
                    }
                }

                @Override
                public void onFailure(int code, Header[] headers, byte[] responseBody, Throwable error) {
                    // Manejar el fallo de la solicitud aquí si es necesario
                    statusCode[0] = code;
                }
            });

            try {
                // Agregar un retardo de 2.5 segundos
                Thread.sleep(2500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } while (statusCode[0] == 0); // Repite mientras el código de estado sea 0
    }



    public void servoOn(View view) {
    }
}