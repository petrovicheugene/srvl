-- Скрипт сгенерирован Devart dbForge Studio for MySQL, Версия 7.1.13.0
-- Домашняя страница продукта: http://www.devart.com/ru/dbforge/mysql/studio
-- Дата скрипта: 01.09.2016 15:28:41
-- Версия сервера: 5.7.14
-- Пожалуйста, сохраните резервную копию Вашей схемы перед запуском этого скрипта 

CREATE DATABASE IF NOT EXISTS sdanalyzer COLLATE utf8_general_ci;
USE sdanalyzer;

# ..\sdanalyzer.sql
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;

--
-- Скрипт сгенерирован Devart dbForge Studio for MySQL, Версия 7.1.13.0
-- Домашняя страница продукта: http://www.devart.com/ru/dbforge/mysql/studio
-- Дата скрипта: 29.08.2016 16:36:19
-- Версия сервера: 5.7.14-log
-- Версия клиента: 4.1
--


--
-- Описание для таблицы calibration_stacks
--
DROP TABLE IF EXISTS calibration_stacks;
CREATE TABLE IF NOT EXISTS calibration_stacks (
  id int(11) NOT NULL,
  name varchar(50) NOT NULL,
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 1
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Calibration stacks for sample calculation'
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы calculation_tasks
--
DROP TABLE IF EXISTS calculation_tasks;
CREATE TABLE IF NOT EXISTS calculation_tasks (
  id int(11) NOT NULL,
  name varchar(50) NOT NULL,
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 1
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Tasks for concentration calculation in a sample'
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы chem_elements
--
DROP TABLE IF EXISTS chem_elements;
CREATE TABLE IF NOT EXISTS chem_elements (
  name varchar(50) NOT NULL,
  PRIMARY KEY (name)
)
ENGINE = INNODB
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Chemical element or composition'
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы measurement_conditions
--
DROP TABLE IF EXISTS measurement_conditions;
CREATE TABLE IF NOT EXISTS measurement_conditions (
  id int(11) NOT NULL,
  gain_factor varchar(255) NOT NULL,
  exposition varchar(255) NOT NULL,
  PRIMARY KEY (exposition, gain_factor),
  UNIQUE INDEX id (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 1
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Sample measurement conditions '
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы samples
--
DROP TABLE IF EXISTS samples;
CREATE TABLE IF NOT EXISTS samples (
  id int(11) NOT NULL,
  name varchar(50) NOT NULL,
  PRIMARY KEY (id),
  UNIQUE INDEX name (name)
)
ENGINE = INNODB
AUTO_INCREMENT = 1
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Measured samples'
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы series_tasks
--
DROP TABLE IF EXISTS series_tasks;
CREATE TABLE IF NOT EXISTS series_tasks (
  id int(11) NOT NULL,
  name varchar(50) NOT NULL,
  PRIMARY KEY (id)
)
ENGINE = INNODB
AUTO_INCREMENT = 1
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Tasks for series measurement'
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы calibrations
--
DROP TABLE IF EXISTS calibrations;
CREATE TABLE IF NOT EXISTS calibrations (
  id int(11) NOT NULL,
  name varchar(50) NOT NULL,
  measurement_condition_id int(11) NOT NULL,
  chem_element varchar(255) NOT NULL,
  PRIMARY KEY (id),
  CONSTRAINT FK_calibrations_chem_elements_name FOREIGN KEY (chem_element)
  REFERENCES chem_elements (name) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT FK_calibrations_measurement_conditions_id FOREIGN KEY (measurement_condition_id)
  REFERENCES measurement_conditions (id) ON DELETE NO ACTION ON UPDATE NO ACTION
)
ENGINE = INNODB
AUTO_INCREMENT = 1
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Calibrations'
ROW_FORMAT = DYNAMIC;

--
-- Описание для таблицы stacked_calibrations
--
DROP TABLE IF EXISTS stacked_calibrations;
CREATE TABLE IF NOT EXISTS stacked_calibrations (
  calibration_id int(11) NOT NULL,
  calibration_stack_id int(11) NOT NULL,
  PRIMARY KEY (calibration_id, calibration_stack_id),
  CONSTRAINT FK_stacked_calibrations_calbration_stacks_id FOREIGN KEY (calibration_stack_id)
  REFERENCES calibration_stacks (id) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT FK_stacked_calibrations_calibrations_id FOREIGN KEY (calibration_id)
  REFERENCES calibrations (id) ON DELETE NO ACTION ON UPDATE NO ACTION
)
ENGINE = INNODB
CHARACTER SET utf8
COLLATE utf8_general_ci
COMMENT = 'Ancillary table for many to many relationship'
ROW_FORMAT = DYNAMIC;

/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;

