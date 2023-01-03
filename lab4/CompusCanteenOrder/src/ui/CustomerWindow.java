package ui;

import application.Customer;
import application.Main;

import javax.swing.*;
import javax.swing.plaf.PanelUI;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;

/**
 * @author LICONG
 */
public class CustomerWindow {
    private Image bg;
    private JPanel mainPanel;
    private JButton orderBt;
    private JButton viewOrderBt;
    private JButton exitBt;
    private JLabel type;
    private JLabel name;
    private Customer customer;

    public CustomerWindow(Customer customer) {
        bg = new ImageIcon("src/images/bg3.jpeg").getImage();
        System.out.println("customer window");
        mainPanel.setUI(new PanelUI() {
            @Override
            public void paint(Graphics g, JComponent c) {
                super.paint(g, c);
                g.drawImage(bg, 0, 0, Main.WIDTH, Main.HEIGHT, mainPanel);
            }
        });
        this.customer = customer;
        name.setText(customer.getName());
        orderBt.setBackground(new Color(0x72C8F6));
        viewOrderBt.setBackground(new Color(0x72C8F6));
        exitBt.setBackground(new Color(0x72C8F6));
        // 点单
        orderBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                try {
                    Main.frame.setContentPane(new OrderWindow(customer).getMainPanel());
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }
            }
        });
        // 查看订单
        viewOrderBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                try {
                    Main.frame.setContentPane(new OrderInfoWindow(customer, null).getMainPanel());
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }
            }
        });
        // 退出登录
        exitBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                Main.frame.setContentPane(new SignInWindow());
            }
        });
    }

    public JPanel getMainPanel() {
        return mainPanel;
    }
}
