-- psql -h <hostname> -d <database name> -U <username> -f mcl_create_table_organism_pub.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Add columns
--
ALTER TABLE analysis 
  ADD COLUMN program VARCHAR(255) NOT NULL,
  ADD COLUMN programversion VARCHAR(255) NOT NULL,
  ADD COLUMN algorithm VARCHAR(255),
  ADD COLUMN sourcename VARCHAR(255),
  ADD COLUMN sourceversion VARCHAR(255),
  ADD COLUMN sourceuri text,
  ADD COLUMN timeexecuted timestamp without time zone NOT NULL DEFAULT now();
--
-- Add UNIQUE
--
ALTER TABLE ONLY analysis ADD CONSTRAINT analysis_c1 UNIQUE (program, programversion, sourcename);
--
-- Add Comments
--
comment on column analysis.program is 'Program name, e.g. blastx, blastp, sim4, genscan.';
comment on column analysis.programversion is 'Version description, e.g. TBLASTX 2.0MP-WashU [09-Nov-2000].';
comment on column analysis.algorithm is 'Algorithm name, e.g. blast..';
comment on column analysis.sourcename is 'Source name, e.g. cDNA, SwissProt.';
comment on column analysis.sourceuri is 'This is an optional, permanent URL or URI for the source of the analysis. The idea is that someone could recreate the analysis directly by going to this URI and fetching the source data (e.g. the blast database, or the training model).';
--
-- Add Default to analysis.analysis_id
-- nextval(''analysis_analysis_id_seq''::regclass)
--ALTER TABLE analysis ALTER COLUMN analysis_id SET DEFAULT '';

--
-- Add Default to analysisfeature.analysisfeature_id
-- nextval('analysisfeature_analysisfeature_id_seq'::regclass)


