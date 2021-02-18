-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_stock_contact.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: feature_contact; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE stock_contact (
    stock_contact_id integer NOT NULL,
    stock_id integer NOT NULL,
    contact_id integer NOT NULL
);
--
-- Name: stock_contact_stock_contact_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE stock_contact_stock_contact_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE stock_contact_stock_contact_id_seq OWNED BY stock_contact.stock_contact_id;
ALTER TABLE ONLY stock_contact ALTER COLUMN stock_contact_id SET DEFAULT nextval('stock_contact_stock_contact_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY stock_contact ADD CONSTRAINT stock_contact_pkey PRIMARY KEY (stock_contact_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY stock_contact ADD CONSTRAINT stock_contact_c1 UNIQUE (stock_id, contact_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY stock_contact ADD CONSTRAINT stock_contact_stock_id_fkey FOREIGN KEY (stock_id) REFERENCES stock(stock_id) ON DELETE CASCADE;
ALTER TABLE ONLY stock_contact ADD CONSTRAINT stock_contact_contact_id_fkey FOREIGN KEY (contact_id) REFERENCES contact(contact_id) ON DELETE CASCADE;
