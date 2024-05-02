package com.example.project.tcpip;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.fxml.FXML;
import javafx.scene.control.TextArea;

import java.io.IOException;
import javax.swing.*;
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
    public Circle red_led;
    public Circle blue_led;
    public ProgressBar progressbar_humidity;
    public ProgressBar progressbar_distance;
    public Label label_distance;
    public Button button_door_open;
    public Button button_door_close;
    public Rectangle distance_led;

    private boolean state_of_button_red_led;
    private boolean state_of_button_blue_led;
    private final Socket socket;

    public AppMainController(){
        this.state_of_button_red_led = false;
        this.state_of_button_blue_led = false;
        this.socket = new Socket();
    }

    public void buttonOnClickedDoorOpen(ActionEvent actionEvent) {
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

    public void buttonOnClickedDoorClose(ActionEvent actionEvent) {
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
            this.received_data_from_server(new ActionEvent());
        } catch (IOException e) {
//            throw new RuntimeException(e);
            System.out.printf("%s\r\n",e.getMessage());
        }
    }
    private void received_data_from_server(ActionEvent event) {
        Thread thread_of_receiving = new Thread(() -> {
            while (true) {
                try {
                    final InputStream inputStream = this.socket.getInputStream();
                    byte[] bytes_data = new byte[512];
                    final int read_byte_count = inputStream.read(bytes_data);
                    final String serial_input_data = new String(bytes_data, 0, read_byte_count, StandardCharsets.UTF_8);
                    final String[] parsings_data = serial_input_data.split(",");
                    if(parsings_data.length != 4) continue;
                    final double temperature = Double.parseDouble(parsings_data[0]);
                    final double humidity = Double.parseDouble(parsings_data[1]);
                    final double distance = Double.parseDouble(parsings_data[2]);
                    final String UID = parsings_data[3];
                    final double changed_temperature = change_progress_value(temperature, 0.0, 40.0, 0.0, 1.0);
                    final double changed_humidity = change_progress_value(humidity, 0.0, 100.0, 0.0, 1.0);
                    final double changed_distance = change_progress_value(distance, 0.0, 60.0, 0.0, 1.0);
                    System.out.printf("%s\r\n",temperature);
                    System.out.printf("%s\r\n",humidity);
                    System.out.printf("%s\r\n",distance);
                    System.out.printf("%s\r\n",UID);

                    Platform.runLater(() -> {
                        if (distance <= 20) {
                            distance_led.setFill(Paint.valueOf("#00FF00"));
                        } else {
                            distance_led.setFill(Paint.valueOf("#FF0000"));
                        }
                    });

                    if (UID.contains("ID ALLOWED")) {
                        Platform.runLater(() -> {
                            blue_led.setFill(Paint.valueOf("blue"));
                            red_led.setFill(Paint.valueOf("black"));
                        });
                    } else if (UID.contains("ID IS NOT ALLOWED")) {
                        Platform.runLater(() -> {
                            red_led.setFill(Paint.valueOf("red"));
                            blue_led.setFill(Paint.valueOf("black"));
                        });
                    }

                    Platform.runLater(() -> {
                        progressbar_temperature.setProgress(changed_temperature);
                        progressbar_humidity.setProgress(changed_humidity);
                        progressbar_distance.setProgress(changed_distance);
                        label_temperature.setText(parsings_data[0]);
                        label_humidity.setText(parsings_data[1]);
                        label_distance.setText(parsings_data[2]);
                    });
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        });
        thread_of_receiving.start();
    }

    private double change_progress_value(double x, double in_min, double in_max, double out_min, double out_max){
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
}