package dao;

import application.CanteenShopFood;

import java.sql.SQLException;
import java.util.List;

/**
 * @author LICONG
 *
 * canteen_shop_food视图的数据访问对象接口
 */
public interface CanteenShopFoodDao {
    /**
     * 获取视图的所有信息
     *
     * @return
     */
    List<CanteenShopFood> getAll() throws SQLException;
}
