package dao;

import application.Shop;

import java.sql.SQLException;
import java.util.List;

/**
 * @author LICONG
 *
 * shop数据访问对象接口
 */
public interface ShopDao {
    /**
     * @param canteen_id
     * @return canteen_id对应食堂中的所有商铺
     * @throws SQLException
     */
    List<Shop> findAllShops(int canteen_id) throws SQLException;

    /**
     * 获取商铺对应的商家号
     * @param shopName
     * @return
     */
    int getMerchantId(String shopName) throws SQLException;
}
