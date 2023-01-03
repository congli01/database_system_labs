package ui;

import application.Customer;
import application.Main;
import application.Merchant;
import application.OrderInfo;
import dao.OrderDao;
import dao.OrderDaoImpl;

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
 * 查看订单信息
 */
public class OrderInfoWindow {
    private Image bg;
    private Customer customer;
    private Merchant merchant;
    private JPanel mainPanel;
    private JTable orderTable;
    private JScrollPane tableScrollPanel;
    private JPanel topPanel;
    private JPanel bottomPanel;
    private JButton returnBt;
    private JButton receiveOrEvaluateBt;
    private DefaultTableModel model;
    private OrderDao orderDao;

    public OrderInfoWindow(Customer customer, Merchant merchant) throws SQLException {
        this.customer = customer;
        this.merchant = merchant;
        orderDao = new OrderDaoImpl();
        bg = new ImageIcon("src/images/bg5.jpg").getImage();
        mainPanel.setUI(new PanelUI() {
            @Override
            public void paint(Graphics g, JComponent c) {
                super.paint(g, c);
                g.drawImage(bg, 0, 0, Main.WIDTH, Main.HEIGHT, mainPanel);
            }
        });
        orderTable.setRowHeight(30);
        receiveOrEvaluateBt.setBackground(new Color(0x72C8F6));
        returnBt.setBackground(new Color(0x72C8F6));
        if (customer != null) {
            receiveOrEvaluateBt.setText("评价");
            display(orderDao.findByCustomer(customer.getCustomerId()));
        } else if(merchant != null){
            receiveOrEvaluateBt.setText("接单");
            display(orderDao.findByMerchant(merchant.getMerchantId()));
        }

        // 点击按钮返回上级界面
        returnBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mainPanel.setVisible(false);
                Main.frame.remove(mainPanel);
                if (customer != null) {
                    Main.frame.setContentPane(new CustomerWindow(customer).getMainPanel());
                } else if(merchant != null) {
                    Main.frame.setContentPane(new MerchantWindow(merchant).getMainPanel());
                }
            }
        });
        // 评价/接单
        receiveOrEvaluateBt.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(merchant != null) {
                    int row = orderTable.getSelectedRow();
                    System.out.println("row:" + row);
                    if(row == -1) {
                        JOptionPane.showMessageDialog(null, "请选择需要操作的订单", "提示", JOptionPane.WARNING_MESSAGE);
                        return;
                    }
                    int orderId = Integer.parseInt((String) orderTable.getValueAt(row, 0));
                    try {
                        orderDao.modifyState(orderId);
                        JOptionPane.showMessageDialog(null, "接单成功", "提示", JOptionPane.YES_NO_CANCEL_OPTION);
                    } catch (SQLException ex) {
                        ex.printStackTrace();
                        return;
                    }
                }
            }
        });
    }

    public JPanel getMainPanel() {
        return mainPanel;
    }

    /**
     * 展示所有订单
     */
    private void display(List<OrderInfo> orderInfoList) {
        String[][] tableData = new String[orderInfoList.size()][7];
        for(int i = 0; i < orderInfoList.size(); i++) {
            tableData[i] = new String[]{String.valueOf(orderInfoList.get(i).getOrderId()),
                    orderInfoList.get(i).getFoodName(), String.valueOf(orderInfoList.get(i).getNum()),
                    orderInfoList.get(i).getBuilding(), orderInfoList.get(i).getRoom(),
                    orderInfoList.get(i).getPhone(), orderInfoList.get(i).getOrderTime(),
                    orderInfoList.get(i).getOrderState()
            };
        }
        String[] columnName = {"订单号", "商品", "数量", "地址", "房间号", "手机号", "下单时间", "订单状态"};

        model = new DefaultTableModel(tableData, columnName) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        orderTable.setModel(model);
        tableScrollPanel.setViewportView(orderTable);
    }
}
