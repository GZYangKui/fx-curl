CREATE TABLE IF NOT EXISTS node_tree
(
    id          integer            not null
        constraint node_tree_pk primary key autoincrement,
    type        smallint default 0 not null,
    parent_id   integer  default 0 not null,
    name        text               not null,
    create_time timestamp          not null,
    method      integer,
    url         text
);



