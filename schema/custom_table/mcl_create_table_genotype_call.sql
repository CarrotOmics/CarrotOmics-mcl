-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_genotype_call.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: genotype_call; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE genotype_call (
    genotype_call_id integer NOT NULL,
    project_id integer NOT NULL,
    feature_id integer NOT NULL,
    stock_id integer NOT NULL,
    genotype_id integer NOT NULL,
    meta_data text
);
--
-- Name: genotype_call_genotype_call_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE genotype_call_genotype_call_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE genotype_call_genotype_call_id_seq OWNED BY genotype_call.genotype_call_id;
ALTER TABLE ONLY genotype_call ALTER COLUMN genotype_call_id SET DEFAULT nextval('genotype_call_genotype_call_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY genotype_call ADD CONSTRAINT genotype_call_a_pkey PRIMARY KEY (genotype_call_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY genotype_call ADD CONSTRAINT genotype_call_c1 UNIQUE (project_id, stock_id, feature_id, genotype_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY genotype_call ADD CONSTRAINT genotype_call_feature_id_fkey FOREIGN KEY (feature_id) REFERENCES feature(feature_id) ON DELETE CASCADE;
ALTER TABLE ONLY genotype_call ADD CONSTRAINT genotype_call_genotype_id_fkey FOREIGN KEY (genotype_id) REFERENCES genotype(genotype_id) ON DELETE CASCADE;
ALTER TABLE ONLY genotype_call ADD CONSTRAINT genotype_call_project_id_fkey FOREIGN KEY (project_id) REFERENCES project(project_id) ON DELETE CASCADE;
ALTER TABLE ONLY genotype_call ADD CONSTRAINT genotype_call_stock_id_fkey FOREIGN KEY (stock_id) REFERENCES stock(stock_id) ON DELETE CASCADE;
