package ui;

import application.*;
import dao.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;

/**
 * @author LICONG
 * 登录窗口
 */
public class SignInWindow extends JPanel {

    private Image image;
    private JRadioButton customerBt;
    private JRadioButton merchantBt;
    private JRadioButton managerBt;
    private JTextField account;
    private JPasswordField password;
    private JButton signInBt;
    private JButton signUpBt;
    private UserType inputType;
    private String inputAccount;
    private String inputPassword;
    private UserDao userDao;
    private CustomerDao customerDao;
    private MerchantDao merchantDao;
    private ManagerDao managerDao;
    private User user;
    private Customer customer;
    private Merchant merchant;
    private Manager manager;

    public SignInWindow() {
        image = new ImageIcon("src/images/bg1.jpg").getImage();
        customerBt = new JRadioButton("顾客");
        merchantBt = new JRadioButton("商家");
        managerBt = new JRadioButton("管理员");
        account = new JTextField(16);
        password = new JPasswordField(16);
        signInBt = new JButton("登录");
        signUpBt = new JButton("新用户注册");
        account.setFont(new Font("宋体", Font.BOLD, 16));
        password.setFont(new Font("宋体", Font.BOLD, 16));
        signInBt.setFont(new Font("宋体", Font.BOLD, 16));
        signUpBt.setFont(new Font("宋体", Font.BOLD, 16));
        this.setLayout(null);
        customerBt.setBounds(300, 225, 50, 20);
        merchantBt.setBounds(450, 225, 50, 20);
        managerBt.setBounds(600, 225, 70, 20);
        account.setBounds(300, 300, 400, 50);
        password.setBounds(300, 400, 400, 50);
        signInBt.setBounds(300, 500, 400, 50);
        signUpBt.setBounds(300, 600, 400, 50);
        signInBt.setBackground(new Color(0x72C8F6));
        signUpBt.setBackground(new Color(0x72C8F6));
        this.add(customerBt);
        this.add(merchantBt);
        this.add(managerBt);
        this.add(account);
        this.add(password);
        this.add(signUpBt);
        this.add(signInBt);
        ButtonGroup group = new ButtonGroup();
        group.add(customerBt);
        group.add(merchantBt);
        group.add(managerBt);
        userDao = new UserDaoImpl();


        // 登录验证
        signInBt.addActionListener(new ActionListener() {
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
                inputPassword = new String(password.getPassword());
                if (inputPassword.equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入密码", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                } else {
                    System.out.println("密码：" + inputPassword);
                }

                // 从数据库获取用户信息
                try {
                    user = userDao.find(inputAccount);
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }
                if (user == null) {
                    JOptionPane.showMessageDialog(null, "用户不存在，请先注册", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                } else {
                    // 进行验证
                    System.out.println("进行验证");

                    // 用户类型验证
                    if (customerBt.isSelected()) {
                        customerDao = new CustomerDaoImpl();
                        try {
                            customer = customerDao.find(inputAccount);
                            if (customer == null) {
                                JOptionPane.showMessageDialog(null, "用户类型错误", "提示", JOptionPane.WARNING_MESSAGE);
                                System.out.println("用户类型错误");
                                return;
                            }
                        } catch (SQLException ex) {
                            ex.printStackTrace();
                        }
                    } else if (merchantBt.isSelected()) {
                        merchantDao = new MerchantDaoImpl();
                        try {
                            merchant = merchantDao.find(inputAccount);
                            if (merchant == null) {
                                JOptionPane.showMessageDialog(null, "用户类型错误", "提示", JOptionPane.WARNING_MESSAGE);
                                System.out.println("用户类型错误");
                                return;
                            }
                        } catch (SQLException ex) {
                            ex.printStackTrace();
                        }
                    } else {
                        managerDao = new ManagerDaoImpl();
                        try {
                            manager = managerDao.find(inputAccount);
                            if (manager == null) {
                                JOptionPane.showMessageDialog(null, "用户类型错误", "提示", JOptionPane.WARNING_MESSAGE);
                                System.out.println("用户类型错误");
                                return;
                            }
                        } catch (SQLException ex) {
                            ex.printStackTrace();
                        }
                    }
                    // 密码验证
                    if (inputPassword.equals(user.getPassword())) {
                        System.out.println("登录成功");
                        System.out.println(inputType);
                        setVisible(false);
                        Main.frame.remove(Main.mLogin);
                        switch (inputType) {
                            case CUSTOMER -> Main.frame.setContentPane(new CustomerWindow(customer).getMainPanel());
                            case MERCHANT -> Main.frame.setContentPane(new MerchantWindow(merchant).getMainPanel());
                            default -> Main.frame.setContentPane(new SignInWindow());
                        }
                    } else {
                        System.out.println("密码错误");
                        JOptionPane.showMessageDialog(null, "密码错误", "提示", JOptionPane.WARNING_MESSAGE);
                    }
                }
            }
        });
        // 进入注册界面
        signUpBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setVisible(false);
                Main.frame.remove(Main.mLogin);
                Main.frame.setContentPane(new SignUpWindow().getMainPanel());
            }
        });

        repaint();
    }



    @Override
    public void paint(Graphics g) {
        super.paint(g);
        g.drawImage(image, 0, 0, this.getWidth(), this.getHeight(), this);
        g.setColor(new Color(0xFF0026));
        g.setFont(new Font("SansSerif", Font.BOLD+Font.ITALIC, 40));
        g.drawString("欢迎光临", 400, 200);
        g.setColor(new Color(0xFFFFFF));
        g.setFont(new Font("宋体", Font.BOLD, 16));
        g.drawString("用户类型", 220, 240);
        g.drawString("账号", 250, 330);
        g.drawString("密码", 250, 430);
        customerBt.requestFocus();
        merchantBt.requestFocus();
        managerBt.requestFocus();
        account.requestFocus();
        password.requestFocus();
        signInBt.requestFocus();
        signUpBt.requestFocus();
    }
}
