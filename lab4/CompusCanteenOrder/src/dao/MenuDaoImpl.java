package dao;

import application.Menu;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * @author LICONG
 */
public class MenuDaoImpl extends BaseDao implements MenuDao{
    @Override
    public boolean add(Menu menu) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.menu(food_id, order_id, number) values(?, ?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setInt(1, menu.getFoodId());
        stmt.setInt(2, menu.getOrderId());
        stmt.setInt(3, menu.getNum());
        boolean result = stmt.execute();
        BaseDao.closeAll(conn, stmt, null);
        return result;
    }
}
