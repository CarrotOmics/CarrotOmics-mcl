-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_organism_pub.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: stockcollection_db; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE stockcollection_db (
    stockcollection_db_id integer NOT NULL,
    stockcollection_id integer NOT NULL,
    db_id integer NOT NULL
);
--
-- Name: stockcollection_db_stockcollection_db_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE stockcollection_db_stockcollection_db_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE stockcollection_db_stockcollection_db_id_seq OWNED BY stockcollection_db.stockcollection_db_id;
ALTER TABLE ONLY stockcollection_db ALTER COLUMN stockcollection_db_id SET DEFAULT nextval('stockcollection_db_stockcollection_db_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY stockcollection_db ADD CONSTRAINT stockcollection_db_a_pkey PRIMARY KEY (stockcollection_db_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY stockcollection_db ADD CONSTRAINT stockcollection_db_c1 UNIQUE (stockcollection_id, db_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY stockcollection_db ADD CONSTRAINT stockcollection_db_stockcollection_id_fkey FOREIGN KEY (stockcollection_id) REFERENCES stockcollection(stockcollection_id) ON DELETE CASCADE;
ALTER TABLE ONLY stockcollection_db ADD CONSTRAINT stockcollection_db_db_id_fkey FOREIGN KEY (db_id) REFERENCES db(db_id) ON DELETE CASCADE;
