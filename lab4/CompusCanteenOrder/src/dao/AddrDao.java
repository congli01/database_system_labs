package dao;

import application.Addr;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * addr的数据访问对象接口
 */
public interface AddrDao {
    int totalNum() throws SQLException;

    boolean add(Addr addr) throws SQLException;
}
