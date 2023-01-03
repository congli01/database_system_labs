package dao;

import application.Merchant;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * 数据访问对象模式
 * merchant的Dao接口
 */
public interface MerchantDao {
    /**
     * 查询数据库中用户总数
     * @return
     */
    int totalNum() throws SQLException;

    Merchant find(String userId) throws SQLException;

    boolean add(Merchant merchant) throws SQLException;
}
