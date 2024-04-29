package com.example.project.tcpip;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Circle;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.ResourceBundle;

public class AppMainController implements Initializable {

    public Label label_temperature;
    public ProgressBar progressbar_temperature;
    public Label label_humidity;
    public ProgressBar Progressbar_humidity;
    public Circle red_led;
    public Circle blue_led;
    public Label label_serial_output; // 새로 추가된 레이블

    private final Socket socket;

    public AppMainController(){
        this.socket = new Socket();
    }

    public void buttonOnClickedRedLED(ActionEvent actionEvent) {
        if (this.socket.isConnected()) {
            byte[] bytes_data = "DOOR_OPEN\n".getBytes(StandardCharsets.UTF_8);
            try {
                final var output_stream = this.socket.getOutputStream();
                output_stream.write(bytes_data);
                output_stream.flush();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    public void buttonOnClickedBlueLED(ActionEvent actionEvent) {
        if (this.socket.isConnected()) {
            byte[] bytes_data = "DOOR_CLOSE\n".getBytes(StandardCharsets.UTF_8);
            try {
                final var output_stream = this.socket.getOutputStream();
                output_stream.write(bytes_data);
                output_stream.flush();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        try {
            this.socket.connect(new InetSocketAddress("192.168.0.8", Integer.parseInt("9999")));
            this.receiveDataFromServer();
        } catch (IOException e) {
            System.out.printf("%s\r\n", e.getMessage());
        }
    }

    private void receiveDataFromServer() {
        Thread threadOfReceiving = new Thread(() -> {
            while (true) {
                try {
                    final InputStream inputStream = this.socket.getInputStream();
                    byte[] bytes_data = new byte[512];
                    final int read_byte_count = inputStream.read(bytes_data);
                    final String serial_input_data = new String(bytes_data, 0, read_byte_count, StandardCharsets.UTF_8);
                    final String[] parsings_data = serial_input_data.split(",");
                    if (parsings_data.length != 3) continue;
                    final double temperature = Double.parseDouble(parsings_data[0]);
                    final double humidity = Double.parseDouble(parsings_data[1]);
                    final double distance = Double.parseDouble(parsings_data[2]);

                    Platform.runLater(() -> {
                        progressbar_temperature.setProgress(temperature / 40.0);
                        Progressbar_humidity.setProgress(humidity / 100.0);
                        label_temperature.setText(String.valueOf(temperature));
                        label_humidity.setText(String.valueOf(humidity));

                        if (distance > 0 && distance <= 20) {
                            red_led.setFill(Paint.valueOf("red"));
                            blue_led.setFill(Paint.valueOf("black"));
                        } else if (distance > 20 && distance <= 50) {
                            red_led.setFill(Paint.valueOf("black"));
                            blue_led.setFill(Paint.valueOf("blue"));
                        } else if (distance > 50) {
                            red_led.setFill(Paint.valueOf("black"));
                            blue_led.setFill(Paint.valueOf("black"));
                        }

                        // 시리얼 출력 결과도 UI에 표시
                        label_serial_output.setText(serial_input_data);
                    });
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        });
        threadOfReceiving.start();
    }
}
