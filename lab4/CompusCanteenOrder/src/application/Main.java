package application;

import ui.SignInWindow;

import javax.swing.*;
import java.awt.*;
import java.sql.*;

public class Main {

    private Image image;
    public static JFrame frame;
    public static SignInWindow mLogin;
    public static String test;
    public static int WIDTH = 1000;
    public static int HEIGHT = 800;

    public static void main(String[] args) {

        frame = new JFrame("校园食堂点餐系统");
        frame.setSize(WIDTH, HEIGHT);
        frame.setLocationRelativeTo(null);
        mLogin = new SignInWindow();
        frame.setContentPane(mLogin);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
        frame.setResizable(false);
    }
}
