package ui;

import application.*;
import application.Menu;
import dao.*;

import javax.swing.*;
import javax.swing.plaf.PanelUI;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;
import java.util.List;

/**
 * @author LICONG
 *
 * 点餐界面
 */
public class OrderWindow {
    private Image bg;
    private Customer customer;
    private JPanel mainPanel;
    private JPanel topPanel;
    private JPanel bottomPanel;
    private JTable foodTable;
    private JScrollPane tableScrollPanel;
    private JTextField inputNum;
    private JLabel numberLabel;
    private JTextField inputAddr;
    private JButton submitBt;
    private JLabel addrLabel;
    private JTextField inputPhone;
    private JLabel phoneLabel;
    private JTextField inputRoom;
    private JLabel roomLabel;
    private JButton returnBt;
    private DefaultTableModel model;
    private CanteenShopFoodDao canteenShopFoodDao;
    private FoodDao foodDao;
    private OrderDao orderDao;
    private AddrDao addrDao;
    private MenuDao menuDao;
    private ShopDao shopDao;

    public OrderWindow(Customer customer) throws SQLException {
        this.customer = customer;
        bg = new ImageIcon("src/images/bg4.jpg").getImage();
        mainPanel.setUI(new PanelUI() {
            @Override
            public void paint(Graphics g, JComponent c) {
                super.paint(g, c);
                g.drawImage(bg, 0, 0, Main.WIDTH, Main.HEIGHT, mainPanel);
            }
        });
        foodTable.setRowHeight(30);
        inputPhone.setText(customer.getPhone());
        submitBt.setBackground(new Color(0x72C8F6));
        returnBt.setBackground(new Color(0x72C8F6));
        display(getAllFoods());
        // 返回上级界面
        returnBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                Main.frame.setContentPane(new CustomerWindow(customer).getMainPanel());
            }
        });
        // 创建订单
        submitBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                //获取输入
                int row = foodTable.getSelectedRow();
                String canteenName = (String) foodTable.getValueAt(row, 0);
//                System.out.println(canteenName);
                String shopName = (String) foodTable.getValueAt(row, 1);
                String foodName = (String) foodTable.getValueAt(row, 2);
                Float foodPrice = Float.parseFloat((String) foodTable.getValueAt(row, 3));
                System.out.println("foodPrice: " + foodPrice);
                String foodDescription = (String) foodTable.getValueAt(row, 4);
                if (inputNum.getText().equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入需要下单的数量", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                int num = 0;
                try {
                    num=Integer.valueOf(inputNum.getText());
                } catch (Exception exception) {
                    //如果抛出异常，返回False
                    JOptionPane.showMessageDialog(null, "下单的数量需为整数", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                Float totalPrice = foodPrice * num;
                if (inputAddr.getText().equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入配送地址", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                String building = inputAddr.getText();
                if (inputAddr.getText().equals("")) {
                    JOptionPane.showMessageDialog(null, "请输入房间号", "提示", JOptionPane.WARNING_MESSAGE);
                    return;
                }
                String room = inputRoom.getText();
                String phone = inputPhone.getText();

                Object[] options ={ "确定", "取消" };  //自定义按钮上的文字
                String text = "订单的总价格为: " + totalPrice + "。\n是否下单？";
                int select = JOptionPane.showOptionDialog(null, text, "提示",JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[0]);
                System.out.println("select: " + select);
                if (select == 1) {
                    return;
                }

                // 向数据库中加入订单信息
                foodDao = new FoodDaoImpl();
                menuDao = new MenuDaoImpl();
                orderDao = new OrderDaoImpl();
                addrDao = new AddrDaoImpl();
                shopDao = new ShopDaoImpl();
                int foodId = 0;
                int orderId = 0;
                int addressId = 0;
                int customerId = customer.getCustomerId();
                int merchantId = 0;
                String state = "已下单";
                try {
                    foodId = foodDao.getFoodId(foodName);
                } catch (SQLException ex) {
                    ex.printStackTrace();
                    return;
                }
                try {
                    orderId = orderDao.totalNum() + 1;
                } catch (SQLException ex) {
                    ex.printStackTrace();
                    return;
                }
                try {
                    addressId = addrDao.totalNum() + 1;
                } catch (SQLException ex) {
                    ex.printStackTrace();
                    return;
                }
                try {
                    merchantId = shopDao.getMerchantId(shopName);
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }

                Menu menu = new Menu(foodId, orderId, num);
                Order order = new Order(orderId, addressId, customerId, merchantId, totalPrice, null,
                                        state, null);
                Addr addr = new Addr(addressId, building, room);

                try {
                    addrDao.add(addr);
                } catch (SQLException ex) {
                    ex.printStackTrace();
                    return;
                }
                try {
                    orderDao.add(order);
                } catch (SQLException ex) {
                    ex.printStackTrace();
                    return;
                }
                try {
                    menuDao.add(menu);
                } catch (SQLException ex) {
                    ex.printStackTrace();
                    return;
                }

                JOptionPane.showMessageDialog(null, "下单成功", "提示", JOptionPane.YES_NO_CANCEL_OPTION);
            }
        });
    }

    public JPanel getMainPanel() {
        return mainPanel;
    }

    private List<CanteenShopFood> getAllFoods() throws SQLException {
        canteenShopFoodDao = new CanteenShopFoodDaoImpl();
        return canteenShopFoodDao.getAll();

    }

    /**
     * 展示所有菜品
     */
    private void display(List<CanteenShopFood> foodDisplayList) {
        String[][] tableData = new String[foodDisplayList.size()][4];
        for(int i = 0; i < foodDisplayList.size(); i++) {
            tableData[i] = new String[]{foodDisplayList.get(i).getCanteenName(), foodDisplayList.get(i).getShopName(),
                    foodDisplayList.get(i).getFoodName(), String.valueOf(foodDisplayList.get(i).getFoodPrice()),
                    foodDisplayList.get(i).getFoodDescription()};
        }
        String[] columnName = {"食堂", "商铺", "商品", "价格", "简介"};

        model = new DefaultTableModel(tableData, columnName) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        foodTable.setModel(model);
        tableScrollPanel.setViewportView(foodTable);
    }
}
