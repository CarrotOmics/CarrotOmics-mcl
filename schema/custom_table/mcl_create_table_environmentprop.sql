-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_environmentprop.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: environmentprop; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE environmentprop (
    environmentprop_id integer NOT NULL,
    environment_id integer NOT NULL,
    type_id integer NOT NULL,
    value text,
    rank integer DEFAULT 0 NOT NULL
);
--
-- Name: environmentprop_environmentprop_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE environmentprop_environmentprop_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE environmentprop_environmentprop_id_seq OWNED BY environmentprop.environmentprop_id;
ALTER TABLE ONLY environmentprop ALTER COLUMN environmentprop_id SET DEFAULT nextval('environmentprop_environmentprop_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY environmentprop ADD CONSTRAINT environmentprop_pkey PRIMARY KEY (environmentprop_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY environmentprop ADD CONSTRAINT environmentprop_c1 UNIQUE (environment_id, type_id, rank);

--
-- Add INDEX
--
CREATE INDEX environmentprop_idx1 ON environmentprop USING btree (environment_id);
CREATE INDEX environmentprop_idx2 ON environmentprop USING btree (type_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY environmentprop ADD CONSTRAINT environmentprop_environment_id_fkey FOREIGN KEY (environment_id) REFERENCES environment(environment_id) ON DELETE CASCADE;
ALTER TABLE ONLY environmentprop ADD CONSTRAINT environmentprop_type_id_fkey FOREIGN KEY (type_id) REFERENCES cvterm(cvterm_id) ON DELETE CASCADE;
