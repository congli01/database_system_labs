package dao;

import application.Shop;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author LICONG
 */
public class ShopDaoImpl extends BaseDao implements ShopDao{
    @Override
    public List<Shop> findAllShops(int canteen_id) throws SQLException {
        Connection conn=BaseDao.getConnection();
        String sql="select * from canteen.shop where canteen_id = \"" + canteen_id + "\"";
        PreparedStatement stmt= conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        List<Shop> shopList=new ArrayList<Shop>();
        while(rs.next()) {
            Shop shop=new Shop(
                    rs.getInt("shop_id"),
                    rs.getInt("canteen_id"),
                    rs.getInt("manager_id"),
                    rs.getString("shop_name")
            );
            shopList.add(shop);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return shopList;
    }

    @Override
    public int getMerchantId(String shopName) throws SQLException {
        int merchantId = 0;
        Connection conn = BaseDao.getConnection();
        String sql = "select merchant_id from canteen.shop where shop_name = \"" + shopName + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        while (rs.next()) {
            merchantId = rs.getInt("merchant_id");
        }
        return merchantId;
    }
}
