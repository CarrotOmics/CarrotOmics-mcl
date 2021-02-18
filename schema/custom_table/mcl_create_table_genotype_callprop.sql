-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_genotype_callprop.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: genotype_callprop; Type: TABLE; Schema: chado; Owner: -; Tablespace:
--
CREATE TABLE genotype_callprop (
    genotype_callprop_id integer NOT NULL,
    genotype_call_id integer NOT NULL,
    type_id integer NOT NULL,
    value text,
    rank integer DEFAULT 0 NOT NULL
);
--
-- Name: genotype_callprop_genotype_callprop_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE genotype_callprop_genotype_callprop_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE genotype_callprop_genotype_callprop_id_seq OWNED BY genotype_callprop.genotype_callprop_id;
ALTER TABLE ONLY genotype_callprop ALTER COLUMN genotype_callprop_id SET DEFAULT nextval('genotype_callprop_genotype_callprop_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY genotype_callprop ADD CONSTRAINT genotype_callprop_a_pkey PRIMARY KEY (genotype_callprop_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY genotype_callprop ADD CONSTRAINT genotype_callprop_c1 UNIQUE (genotype_call_id, type_id, rank);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY genotype_callprop ADD CONSTRAINT genotype_callprop_genotype_call_id_fkey FOREIGN KEY (genotype_call_id) REFERENCES genotype_call(genotype_call_id) ON DELETE CASCADE;
