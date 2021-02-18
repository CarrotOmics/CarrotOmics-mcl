-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_feature_contact.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: feature_contact; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE feature_contact (
    feature_contact_id integer NOT NULL,
    feature_id integer NOT NULL,
    contact_id integer NOT NULL
);
--
-- Name: feature_contact_feature_contact_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE feature_contact_feature_contact_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE feature_contact_feature_contact_id_seq OWNED BY feature_contact.feature_contact_id;
ALTER TABLE ONLY feature_contact ALTER COLUMN feature_contact_id SET DEFAULT nextval('feature_contact_feature_contact_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY feature_contact ADD CONSTRAINT feature_contact_pkey PRIMARY KEY (feature_contact_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY feature_contact ADD CONSTRAINT feature_contact_c1 UNIQUE (feature_id, contact_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY feature_contact ADD CONSTRAINT feature_contact_feature_id_fkey FOREIGN KEY (feature_id) REFERENCES feature(feature_id) ON DELETE CASCADE;
ALTER TABLE ONLY feature_contact ADD CONSTRAINT feature_contact_contact_id_fkey FOREIGN KEY (contact_id) REFERENCES contact(contact_id) ON DELETE CASCADE;
