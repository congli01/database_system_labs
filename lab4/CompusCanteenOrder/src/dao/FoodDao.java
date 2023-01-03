package dao;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * food的数据访问对象接口
 */
public interface FoodDao {
    int getFoodId(String foodName) throws SQLException;
}
