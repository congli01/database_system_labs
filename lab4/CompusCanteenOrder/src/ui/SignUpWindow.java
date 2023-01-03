package ui;

import application.*;
import dao.*;

import javax.swing.*;
import javax.swing.plaf.PanelUI;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;

/**
 * @author LICONG
 * 用户注册
 */
public class SignUpWindow {
    private JPanel mainPanel;
    private JTextField account;
    private JPasswordField password;
    private JPasswordField confirmPassword;
    private JLabel accountLabel;
    private JLabel passwordLabel;
    private JLabel confirmPasswordLabel;
    private JButton signUpButton;
    private JRadioButton customerBt;
    private JRadioButton merchantBt;
    private JRadioButton managerBt;
    private JLabel userTypeLabel;
    private JTextField phone;
    private JLabel phoneLabel;
    private JButton returnButton;
    private JTextField name;
    private JLabel nameLabel;
    private JLabel sexLabel;
    private JRadioButton maleBt;
    private JRadioButton femaleBt;
    private Image bg;
    private UserType inputType;
    private String inputAccount;
    private String inputName;
    private String inputSex;
    private String inputPhone;
    private String inputPassword;
    private String inputConfirmPassword;
    private UserDao userDao;
    private CustomerDao customerDao;
    private MerchantDao merchantDao;
    private ManagerDao managerDao;

    public SignUpWindow() {
        bg = new ImageIcon("src/images/bg2.jpg").getImage();
        mainPanel.setUI(new PanelUI() {
            @Override
            public void paint(Graphics g, JComponent c) {
                super.paint(g, c);
                g.drawImage(bg, 0, 0, Main.WIDTH, Main.HEIGHT, mainPanel);
            }
        });
        ButtonGroup group1 = new ButtonGroup();
        group1.add(customerBt);
        group1.add(merchantBt);
        group1.add(managerBt);
        ButtonGroup group2 = new ButtonGroup();
        group2.add(maleBt);
        group2.add(femaleBt);
        userTypeLabel.setForeground(Color.WHITE);
        accountLabel.setForeground(Color.WHITE);
        nameLabel.setForeground(Color.WHITE);
        sexLabel.setForeground(Color.WHITE);
        phoneLabel.setForeground(Color.BLACK);
        passwordLabel.setForeground(Color.WHITE);
        confirmPasswordLabel.setForeground(Color.WHITE);
        returnButton.setBackground(new Color(0x72C8F6));
        signUpButton.setBackground(new Color(0x72C8F6));

        // 返回登录界面
        returnButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                Main.frame.setContentPane(new SignInWindow());
            }
        });
        // 进行注册
        signUpButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // 获取输入
                if (customerBt.isSelected()) {
                    inputType = UserType.CUSTOMER;
                } else if (merchantBt.isSelected()) {
                    inputType = UserType.MERCHANT;
                } else if (managerBt.isSelected()) {
                    inputType = UserType.MANAGER;
                } else {
                    JOptionPane.showMessageDialog(null, "请选择用户类型", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                System.out.println("用户类型：" + inputType);

                inputAccount = account.getText();
                if (inputAccount.equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入账号", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                } else {
                    System.out.println("账号：" + inputAccount);
                }

                inputName = name.getText();
                if (inputName.equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入姓名", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                } else {
                    System.out.println("姓名：" + inputName);
                }

                if (maleBt.isSelected()) {
                    inputSex = "男";
                } else if (femaleBt.isSelected()) {
                    inputSex = "女";
                } else {
                    JOptionPane.showMessageDialog(null, "请选择您的性别", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                System.out.println("性别：" + inputSex);

                if(inputType == UserType.CUSTOMER) {
                    inputPhone = phone.getText();
                    if (inputPhone.equals("")) {
                        JOptionPane.showMessageDialog(null, "请输入电话号码", "提示", JOptionPane.WARNING_MESSAGE);
                        return;
                    } else {
                        System.out.println("电话号码" + inputPhone);
                    }
                }

                inputPassword = new String(password.getPassword());
                if (inputPassword.equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入密码", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                } else {
                    System.out.println("密码：" + inputPassword);
                }

                inputConfirmPassword = new String(confirmPassword.getPassword());
                if (inputConfirmPassword.equals("")) {
                    JOptionPane.showMessageDialog(null, "请确认密码", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                } else {
                    System.out.println("确认密码：" + inputConfirmPassword);
                }

                if (!inputPassword.equals(inputConfirmPassword)) {
                    JOptionPane.showMessageDialog(null, "两次输入的密码不一致", "提示", JOptionPane.WARNING_MESSAGE);
                    System.out.println("两次输入密码不一致");
                    return;
                }

                // 向数据库新增数据
                userDao = new UserDaoImpl();
                try {
                    if(userDao.find(inputAccount) != null) {
                        JOptionPane.showMessageDialog(null, "账号已被注册", "提示", JOptionPane.WARNING_MESSAGE);
                        System.out.println("账号已被注册");
                        return;
                    }
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }
                try {
                    userDao.add(new User(inputAccount, inputPassword, inputType));
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }
                switch (inputType) {
                    case CUSTOMER -> {
                        customerDao = new CustomerDaoImpl();
                        try {
                            customerDao.add(new Customer(customerDao.totalNum()+1, inputAccount, inputPhone, inputSex, inputName));
                        } catch (SQLException ex) {
                            ex.printStackTrace();
                        }
                    }
                    case MERCHANT -> {
                        merchantDao = new MerchantDaoImpl();
                        try {
                            merchantDao.add(new Merchant(merchantDao.totalNum()+1, inputAccount, inputName));
                        } catch (SQLException ex) {
                            ex.printStackTrace();
                        }
                    }
                    case MANAGER -> {
                        managerDao = new ManagerDaoImpl();
                        try {
                            managerDao.add(new Manager(managerDao.totalNum()+1, inputAccount, inputName));
                        } catch (SQLException ex) {
                            ex.printStackTrace();
                        }
                    }
                }

                JOptionPane.showMessageDialog(null, "注册成功", "提示", JOptionPane.YES_NO_CANCEL_OPTION);
            }
        });
    }

    public JPanel getMainPanel() {
        return mainPanel;
    }
}
