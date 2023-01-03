package dao;

import application.User;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * @author LICONG
 * user类Dao接口的实现
 */
public class UserDaoImpl extends BaseDao implements UserDao{
    @Override
    public User find(String userId) throws SQLException {
        User user = null;
        Connection conn = BaseDao.getConnection();
        String sql = "select * from canteen.userinfo where user_id = \"" + userId + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        if (rs == null) {
            return user;
        }
        while (rs.next()) {
            user = new User(rs.getString("user_id"),
                    rs.getString("password"),
                    null);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return user;
    }

    @Override
    public boolean add(User user) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.userinfo(user_id, password) values(?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setString(1, user.getUserId());
        stmt.setString(2, user.getPassword());
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }
}
