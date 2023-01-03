package application;

/**
 * @author LICONG
 *
 * 地址类
 */
public class Addr {
    private int addrId;
    private String building;
    private String room;

    public Addr(int addrId, String building, String room) {
        this.addrId = addrId;
        this.building = building;
        this.room = room;
    }

    public int getAddrId() {
        return addrId;
    }

    public String getBuilding() {
        return building;
    }

    public String getRoom() {
        return room;
    }
}
