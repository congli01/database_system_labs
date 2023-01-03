package dao;

import application.Menu;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * menu的数据访问对象接口
 */
public interface MenuDao {
    boolean add(Menu menu) throws SQLException;
}
