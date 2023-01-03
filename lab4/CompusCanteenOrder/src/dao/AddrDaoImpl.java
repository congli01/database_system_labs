package dao;

import application.Addr;

import java.sql.*;

public class AddrDaoImpl extends BaseDao implements AddrDao{
    @Override
    public int totalNum() throws SQLException {
        int num = 0;
        Connection conn = BaseDao.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery("select count(*) as result from canteen.addr");
        while (rs.next()) {
            num = rs.getInt(1);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return num;
    }

    @Override
    public boolean add(Addr addr) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.addr(address_id, building, room) values(?, ?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setInt(1, addr.getAddrId());
        stmt.setString(2, addr.getBuilding());
        stmt.setString(3, addr.getRoom());
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }
}
