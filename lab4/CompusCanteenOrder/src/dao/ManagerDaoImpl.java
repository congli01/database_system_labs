package dao;

import application.Customer;
import application.Manager;

import java.sql.*;

/**
 * @author LICONG
 *
 * manager数据访问对象接口的实现
 */
public class ManagerDaoImpl extends BaseDao implements ManagerDao{
    @Override
    public int totalNum() throws SQLException {
        int num = 0;
        Connection conn = BaseDao.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery("select count(*) as result from canteen.manager");
        while (rs.next()) {
            num = rs.getInt(1);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return num;
    }

    @Override
    public Manager find(String userId) throws SQLException {
        Manager manager = null;
        Connection conn = BaseDao.getConnection();
        String sql = "select * from canteen.manager where user_id = " + userId;
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        if (rs == null) {
            BaseDao.closeAll(conn, stmt, rs);
            return manager;
        }
        while (rs.next()) {
            manager = new Manager(Integer.parseInt(rs.getString("manager_id")),
                    rs.getString("user_id"),
                    rs.getString("manager_name"));
        }
        BaseDao.closeAll(conn, stmt, rs);
        return manager;
    }

    @Override
    public boolean add(Manager manager) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.manager(manager_id, user_id, manager_name) " +
                "values(?, ?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setInt(1, manager.getManagerId());
        stmt.setString(2, manager.getUserId());
        stmt.setString(3, manager.getName());
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }
}
