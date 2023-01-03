package ui;

import application.Main;
import application.Merchant;

import javax.swing.*;
import javax.swing.plaf.PanelUI;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;

/**
 * @author LICONG
 *
 * 商家界面
 */
public class MerchantWindow {
    private Image bg;
    private JButton viewOrderBt;
    private JPanel mainPanel;
    private JButton exitBt;
    private JLabel type;
    private JLabel name;
    private Merchant merchant;

    public MerchantWindow(Merchant merchant) {
        bg = new ImageIcon("src/images/bg3.jpeg").getImage();
        System.out.println("merchant window");
        mainPanel.setUI(new PanelUI() {
            @Override
            public void paint(Graphics g, JComponent c) {
                super.paint(g, c);
                g.drawImage(bg, 0, 0, Main.WIDTH, Main.HEIGHT, mainPanel);
            }
        });
        this.merchant = merchant;
        name.setText(merchant.getName());
        viewOrderBt.setBackground(new Color(0x72C8F6));
        exitBt.setBackground(new Color(0x72C8F6));

        // 查看订单
        viewOrderBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                try {
                    Main.frame.setContentPane(new OrderInfoWindow(null, merchant).getMainPanel());
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
