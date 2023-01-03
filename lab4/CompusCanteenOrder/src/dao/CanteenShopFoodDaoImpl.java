package dao;

import application.CanteenShopFood;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author LICONG
 */
public class CanteenShopFoodDaoImpl extends BaseDao implements CanteenShopFoodDao {
    @Override
    public List<CanteenShopFood> getAll() throws SQLException {
        Connection conn=BaseDao.getConnection();
        String sql="select * from canteen.canteenshopfoodinfo";
        PreparedStatement stmt= conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        List<CanteenShopFood> canteenShopFoodList=new ArrayList<CanteenShopFood>();
        while(rs.next()) {
            CanteenShopFood canteenShopFood=new CanteenShopFood(
                    rs.getString("canteen_name"),
                    rs.getString("shop_name"),
                    rs.getString("food_name"),
                    Float.parseFloat(rs.getString("food_price")),
                    rs.getString("food_description")
            );
            canteenShopFoodList.add(canteenShopFood);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return canteenShopFoodList;
    }
}
