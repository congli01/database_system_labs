package dao;


import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * @author LICONG
 */
public class FoodDaoImpl extends BaseDao implements FoodDao{
    @Override
    public int getFoodId(String foodName) throws SQLException {
        int foodId = 0;
        Connection conn = BaseDao.getConnection();
        String sql = "select food_id from canteen.food where food_name = \"" + foodName + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        while (rs.next()) {
            foodId = rs.getInt("food_id");
        }
        return foodId;
    }
}
