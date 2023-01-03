/* Created on:     2022/12/14 17:13:31                          */
/*==============================================================*/


/*drop trigger od_BEFORE_INSERT;

drop
table if exists orderInfo;

drop
table if exists canteenShopFoodInfo;

drop index idx_food_name on food;*/

drop schema if exists canteen;
create schema canteen;
use canteen;

/*==============================================================*/
/* Table: addr                                                  */
/*==============================================================*/
create table addr
(
   address_id           numeric(10,0) not null,
   building             varchar(128) not null,
   room                 varchar(64) not null,
   primary key (address_id)
);

/*==============================================================*/
/* Table: canteen                                               */
/*==============================================================*/
create table canteen
(
   canteen_id           numeric(8,0) not null,
   manager_id           numeric(8,0),
   canteen_name         varchar(16),
   primary key (canteen_id)
);

/*==============================================================*/
/* Table: customer                                              */
/*==============================================================*/
create table customer
(
   customer_id          numeric(8,0) not null,
   user_id              varchar(16) not null,
   phone_number         numeric(11,0) not null,
   sex                  varchar(2),
   customer_name        varchar(16),
   primary key (customer_id)
);

/*==============================================================*/
/* Table: food                                                  */
/*==============================================================*/
create table food
(
   food_id              numeric(8,0) not null,
   food_name            varchar(16) not null,
   food_price           float(10,0) not null,
   food_description     varchar(256),
   primary key (food_id)
);

/*==============================================================*/
/* Index: idx_food_name                                         */
/*==============================================================*/
create index idx_food_name on food
(
   food_name
);

/*==============================================================*/
/* Table: manager                                               */
/*==============================================================*/
create table manager
(
   manager_id           numeric(8,0) not null,
   user_id              varchar(16) not null,
   manager_name         varchar(16),
   primary key (manager_id)
);

/*==============================================================*/
/* Table: menu                                                  */
/*==============================================================*/
create table menu
(
   food_id              numeric(8,0) not null,
   order_id             numeric(8,0) not null,
   number               int not null,
   primary key (food_id, order_id)
);

/*==============================================================*/
/* Table: merchant                                              */
/*==============================================================*/
create table merchant
(
   merchant_id          numeric(8,0) not null,
   user_id              varchar(16) not null,
   merchant_name        varchar(16),
   primary key (merchant_id)
);

/*==============================================================*/
/* Table: od                                                    */
/*==============================================================*/
create table od
(
   order_id             numeric(8,0) not null,
   address_id           numeric(10,0),
   customer_id          numeric(8,0),
   merchant_id          numeric(8,0),
   order_price          float(8,2),
   order_time           datetime,
   order_state          varchar(16),
   order_evaluation     varchar(256),
   primary key (order_id)
);

/*==============================================================*/
/* Table: shop                                                  */
/*==============================================================*/
create table shop
(
   shop_id              numeric(8,0) not null,
   canteen_id           numeric(8,0),
   merchant_id          numeric(8,0),
   shop_name            varchar(16),
   primary key (shop_id)
);

/*==============================================================*/
/* Table: shop_food                                             */
/*==============================================================*/
create table shop_food
(
   food_id              numeric(8,0) not null,
   shop_id              numeric(8,0) not null,
   primary key (food_id, shop_id)
);

/*==============================================================*/
/* Table: userInfo                                              */
/*==============================================================*/
create table userInfo
(
   user_id              varchar(16) not null,
   password             varchar(16) not null,
   primary key (user_id)
);

/*==============================================================*/
/* View: canteenShopFoodInfo                                    */
/*==============================================================*/
create VIEW  canteenShopFoodInfo
 as
select canteen.canteen_name, shop.shop_name, food.food_name, food.food_price, food.food_description
from canteen, shop, food, shop_food
where canteen.canteen_id = shop.canteen_id and shop.shop_id = shop_food.shop_id
      and shop_food.food_id = food.food_id;

/*==============================================================*/
/* View: orderInfo                                              */
/*==============================================================*/
create VIEW  orderInfo
 as
select od.order_id, od.merchant_id, od.order_state, od.order_time, shop.shop_name, canteen.canteen_name, food.food_name, menu.number, 
       customer.customer_id, customer.customer_name, customer.phone_number, addr.building, addr.room
from od, canteen, shop, food, menu, customer, addr
where od.order_id = menu.order_id and od.merchant_id = shop.merchant_id and shop.canteen_id = canteen.canteen_id and
      menu.food_id = food.food_id and od.customer_id = customer.customer_id and od.address_id = addr.address_id;

alter table canteen add constraint FK_canteen_manager foreign key (manager_id)
      references manager (manager_id) on delete restrict on update restrict;

alter table customer add constraint FK_user_customer foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table manager add constraint FK_user_manager foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table menu add constraint FK_menu foreign key (order_id)
      references od (order_id) on delete restrict on update restrict;

alter table menu add constraint FK_menu2 foreign key (food_id)
      references food (food_id) on delete restrict on update restrict;

alter table merchant add constraint FK_user_merchant foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table od add constraint FK_order_address foreign key (address_id)
      references addr (address_id) on delete restrict on update restrict;

alter table od add constraint FK_商家_订单 foreign key (merchant_id)
      references merchant (merchant_id) on delete restrict on update restrict;

alter table od add constraint FK_顾客_订单 foreign key (customer_id)
      references customer (customer_id) on delete restrict on update restrict;

alter table shop add constraint FK_canteen_shop foreign key (canteen_id)
      references canteen (canteen_id) on delete restrict on update restrict;

alter table shop add constraint FK_shop_merchant foreign key (merchant_id)
      references merchant (merchant_id) on delete restrict on update restrict;

alter table shop_food add constraint FK_shop_food foreign key (food_id)
      references food (food_id) on delete restrict on update restrict;

alter table shop_food add constraint FK_shop_food2 foreign key (shop_id)
      references shop (shop_id) on delete restrict on update restrict;


create trigger od_before_insert
    before insert on od
    for each row
    set new.order_time = now();

