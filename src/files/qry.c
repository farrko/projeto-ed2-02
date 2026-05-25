#include "files/qry.h"
#include "files/svg.h"
#include "datast/exhash.h"
#include "objects/address.h"
#include "objects/block.h"
#include "objects/people.h"
#include "shapes/text.h"
#include "shapes/point.h"
#include "shapes/shapes.h"
#include "datast/linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

point_t *calc_address_pos(address_t *ads, block_t *block) {
  char face = ads_get_face(ads);
  int number = ads_get_number(ads);
  
  double x = block_get_x(block);
  double y = block_get_y(block);

  switch(face) {
    case 'N':
    case 'n':
      x = x + number;
      y = y + block_get_height(block);
      break;
    case 'S':
    case 's':
      x = x + number;
      y = y;
      break;
    case 'L':
    case 'l':
      x = x;
      y = y + number;
      break;
    case 'O':
    case 'o':
      x = x + block_get_width(block);
      y = y + number;
      break;
  }

  return point_init(x, y);
}

void command_rq(char cep[16], exhash_t *blocks, exhash_t *addresses, exhash_t *people, FILE *txt, llist_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO RQ --- argumentos: %s --- \n\n", cep);

  block_t *block = exh_remove(blocks, cep);
  if (block == NULL) return;

  text_t *x = text_init(0, block_get_x(block), block_get_y(block), "start", "#880808", "#000000", "sans-serif", "normal", "medium", "X");
  llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, x)));

  size_t ads_length = exh_entries_amount(addresses);
  uint8_t *ads_list = exh_get_all(addresses);

  for (size_t i = 0; i < ads_length; i++) {
    address_t *current_address = (address_t *)(ads_list + i * ads_sizeof());

    if (strncmp(ads_get_cep(current_address), cep, 15) == 0) {
      address_t *ads = exh_remove(addresses, ads_get_cpf(current_address));
      ads_destroy(ads);

      people_t *person = exh_get(people, ads_get_cpf(current_address));
      fprintf(txt, "- Pessoa despejada:\n\t- Nome: %s %s\n\t- CPF: %s\n\n", people_get_name(person), people_get_surname(person), people_get_cpf(person));

      people_destroy(person);
    }
  }

  block_destroy(block);
  free(ads_list);
}

void command_pq(char cep[16], exhash_t *blocks, exhash_t *addresses, FILE *txt, llist_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO PQ --- argumentos: %s ---\n\n", cep);

  block_t *block = exh_get(blocks, cep);
  if (block == NULL) {
    fprintf(txt, "Quadra não encontrada.\n");
    return;
  }

  uint16_t total_addresses = exh_entries_amount(addresses);
  address_t *address_list = exh_get_all(addresses);

  uint16_t count_n = 0;
  uint16_t count_s = 0;
  uint16_t count_l = 0;
  uint16_t count_o = 0;

  for (uint16_t i = 0; i < total_addresses; i++) {
    address_t *ads = (address_t *)((uint8_t *)address_list + i * ads_sizeof());

    if (strncmp(ads_get_cep(ads), cep, 15) != 0) continue;

    char face = ads_get_face(ads);

    switch(face) {
      case 'N':
      case 'n':
        count_n++;
        break;
      case 'S':
      case 's':
        count_s++;
        break;
      case 'L':
      case 'l':
        count_l++;
        break;
      case 'O':
      case 'o':
        count_o++;
        break;
    }
  }

  uint16_t total = count_n + count_s + count_l + count_o;

  fprintf(txt, "- Face N: %d morador(es)\n", count_n);
  fprintf(txt, "- Face S: %d morador(es)\n", count_s);
  fprintf(txt, "- Face L: %d morador(es)\n", count_l);
  fprintf(txt, "- Face O: %d morador(es)\n", count_o);
  fprintf(txt, "- Total:  %d morador(es)\n", total);

  double bx = block_get_x(block);
  double by = block_get_y(block);
  double bw = block_get_width(block);
  double bh = block_get_height(block);

  char buf[16];

  snprintf(buf, sizeof(buf), "%d", count_n);
  text_t *text_n = text_init(0, bx + bw / 2, by, "middle", "#000000", "#000000", "sans-serif", "normal", "10px", buf);
  llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, text_n)));

  snprintf(buf, sizeof(buf), "%d", count_s);
  text_t *text_s = text_init(0, bx + bw / 2, by + bh, "middle", "#000000", "#000000", "sans-serif", "normal", "10px", buf);
  llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, text_s)));

  snprintf(buf, sizeof(buf), "%d", count_l);
  text_t *text_l = text_init(0, bx, by + bh / 2, "start", "#000000", "#000000", "sans-serif", "normal", "10px", buf);
  llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, text_l)));

  snprintf(buf, sizeof(buf), "%d", count_o);
  text_t *text_o = text_init(0, bx + bw, by + bh / 2, "end", "#000000", "#000000", "sans-serif", "normal", "10px", buf);
  llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, text_o)));

  snprintf(buf, sizeof(buf), "%d", total);
  text_t *text_total = text_init(0, bx + bw / 2, by + bh / 2, "middle", "#000000", "#000000", "sans-serif", "bold", "12px", buf);
  llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, text_total)));

  free(address_list);
  free(block);
}

void command_censo(exhash_t *people, exhash_t *addresses, FILE *txt) {
  fprintf(txt, "\n\n--- COMANDO CENSO ---\n\n");

  size_t total_people = exh_entries_amount(people);
  if (total_people == 0) {
    fprintf(txt, "Nenhum habitante registrado.\n");
    return;
  }

  people_t *people_list = exh_get_all(people);

  size_t total_residents = 0;
  size_t male_inhabitants = 0;
  size_t female_inhabitants = 0;
  size_t male_residents = 0;
  size_t female_residents = 0;

  for (size_t i = 0; i < total_people; i++) {
    people_t *person = (people_t *) ((uint8_t *) people_list + i * people_sizeof());

    char gender = people_get_gender(person);
    if (gender == 'M' || gender == 'm') {
      male_inhabitants++;
    } else female_inhabitants++;

    address_t *ads = exh_get(addresses, people_get_cpf(person));
    if (ads != NULL) {
      total_residents++;
      if (gender == 'M' || gender == 'm') {
        male_residents++;
      } else female_residents++;

      ads_destroy(ads);
    }
  }

  size_t total_homeless = total_people - total_residents;
  size_t male_homeless = male_inhabitants - male_residents;
  size_t female_homeless = female_inhabitants - female_residents;

  double pct_residents = (double) total_residents / total_people * 100.0;
  double pct_male_inhabitants = (double) male_inhabitants / total_people * 100.0;
  double pct_fem_inhabitants = (double) female_inhabitants / total_people * 100.0;
  double pct_male_residents = total_residents > 0 ? (double) male_residents / total_residents * 100.0 : 0.0;
  double pct_fem_residents = total_residents > 0 ? (double) female_residents / total_residents * 100.0 : 0.0;
  double pct_male_homeless = total_homeless  > 0 ? (double) male_homeless / total_homeless  * 100.0 : 0.0;
  double pct_fem_homeless = total_homeless  > 0 ? (double) female_homeless / total_homeless  * 100.0 : 0.0;

  fprintf(txt, "- Total de habitantes: %lu\n", total_people);
  fprintf(txt, "- Total de moradores: %lu\n", total_residents);
  fprintf(txt, "- Proporção moradores/habit.: %.2f%%\n", pct_residents);
  fprintf(txt, "- Número de homens: %lu\n", male_inhabitants);
  fprintf(txt, "- Número de mulheres: %lu\n", female_inhabitants);
  fprintf(txt, "- %% habitantes homens: %.2f%%\n", pct_male_inhabitants);
  fprintf(txt, "- %% habitantes mulheres: %.2f%%\n", pct_fem_inhabitants);
  fprintf(txt, "- %% moradores homens: %.2f%%\n", pct_male_residents);
  fprintf(txt, "- %% moradores mulheres: %.2f%%\n", pct_fem_residents);
  fprintf(txt, "- Total de sem-tetos: %lu\n", total_homeless);
  fprintf(txt, "- %% sem-tetos homens: %.2f%%\n", pct_male_homeless);
  fprintf(txt, "- %% sem-tetos mulheres: %.2f%%\n", pct_fem_homeless);

  free(people_list);
}

void command_h(char cpf[16], exhash_t *people, exhash_t *addresses, FILE *txt) {
  fprintf(txt, "\n\n--- COMANDO H? --- argumentos: %s ---\n\n", cpf);

  people_t *person = exh_get(people, cpf);
  if (person == NULL) {
    fprintf(txt, "Habitante não encontrado.\n");
    return;
  }

  fprintf(txt, "- CPF: %s\n", people_get_cpf(person));
  fprintf(txt, "- Nome: %s %s\n", people_get_name(person), people_get_surname(person));
  fprintf(txt, "- Sexo: %c\n", people_get_gender(person));
  fprintf(txt, "- Nascimento: %s\n", people_get_date_of_birth(person));

  address_t *ads = exh_get(addresses, cpf);
  if (ads != NULL) {
    fprintf(txt, "- Endereço: %s/%c/%d (%s)\n", ads_get_cep(ads), ads_get_face(ads), ads_get_number(ads), ads_get_complement(ads));
    ads_destroy(ads);
  } else {
    fprintf(txt, "- Sem endereço (sem-teto)\n");
  }

  people_destroy(person);
}

void command_nasc(char cpf[16], char name[32], char surname[32], char gender, char dob[11], exhash_t *people, FILE *txt) {
  fprintf(txt, "\n\n--- COMANDO NASC --- argumentos: %s %s %s %c %s ---\n\n", cpf, name, surname, gender, dob);

  people_t *existing = exh_get(people, cpf);
  if (existing != NULL) {
    fprintf(txt, "Habitante com CPF %s já existe.\n", cpf);
    people_destroy(existing);
    return;
  }

  people_t *person = people_init(cpf, name, surname, gender, dob);
  exh_insert(people, cpf, person);
  people_destroy(person);
}

void command_rip(char cpf[16], exhash_t *people, exhash_t *addresses, exhash_t *blocks, FILE *txt, llist_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO RIP --- argumentos: %s ---\n\n", cpf);

  people_t *person = exh_remove(people, cpf);
  if (person == NULL) {
    fprintf(txt, "Habitante não encontrado.\n");
    return;
  }

  fprintf(txt, "- CPF: %s\n", people_get_cpf(person));
  fprintf(txt, "- Nome: %s %s\n", people_get_name(person), people_get_surname(person));
  fprintf(txt, "- Sexo: %c\n", people_get_gender(person));
  fprintf(txt, "- Nascimento: %s\n", people_get_date_of_birth(person));

  address_t *ads = exh_remove(addresses, cpf);
  if (ads != NULL) {
    fprintf(txt, "- Endereço: %s/%c/%d (%s)\n", ads_get_cep(ads), ads_get_face(ads), ads_get_number(ads), ads_get_complement(ads));

    block_t *block = exh_get(blocks, ads_get_cep(ads));
    if (block != NULL) {
      point_t *pos = calc_address_pos(ads, block);

      double cx = point_get_x(pos);
      double cy = point_get_y(pos);

      text_t *cross = text_init(0, cx, cy, "middle", "#FF0000", "#FF0000", "sans-serif", "bold", "12px", "+");
      llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, cross)));

      point_destroy(pos);
      block_destroy(block);
    }

    ads_destroy(ads);
  } else {
    fprintf(txt, "- Sem endereço (sem-teto)\n");
  }

  people_destroy(person);
}

void command_mud(char cpf[16], char cep[16], char face, uint16_t num, char complement[32], exhash_t *people, exhash_t *addresses, exhash_t *blocks, FILE *txt, llist_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO MUD --- argumentos: %s %s %c %u %s ---\n\n", cpf, cep, face, num, complement);

  people_t *person = exh_get(people, cpf);
  if (person == NULL) {
    fprintf(txt, "Habitante não encontrado.\n");
    return;
  }

  address_t *old_ads = exh_remove(addresses, cpf);
  if (old_ads != NULL) ads_destroy(old_ads);

  address_t *new_ads = ads_init(cpf, cep, face, num, complement);
  exh_insert(addresses, cpf, new_ads);

  block_t *block = exh_get(blocks, cep);
  if (block != NULL) {
    point_t *pos = calc_address_pos(new_ads, block);

    double cx = point_get_x(pos);
    double cy = point_get_y(pos);
    double half = 5.0;

    rectangle_t *rect = rect_init(0, cx - half, cy - half, half * 2, half * 2, "#FF0000", "#FF0000", "2px");
    llist_insertat_start(added_elements, shape_as_node(shape_init(RECTANGLE, rect)));

    text_t *label = text_init(0, cx, cy + half, "middle", "#FFFFFF", "#000000", "sans-serif", "normal", "8px", cpf);
    llist_insertat_start(added_elements, shape_as_node(shape_init(TEXT, label)));

    point_destroy(pos);
    block_destroy(block);
  }

  ads_destroy(new_ads);
  people_destroy(person);
}

void command_dspj(char cpf[16], exhash_t *people, exhash_t *addresses, exhash_t *blocks, FILE *txt, llist_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO DSPJ --- argumentos: %s ---\n\n", cpf);

  people_t *person = exh_get(people, cpf);
  if (person == NULL) {
    fprintf(txt, "Habitante não encontrado.\n");
    return;
  }

  address_t *ads = exh_remove(addresses, cpf);
  if (ads == NULL) {
    fprintf(txt, "Habitante não é morador.\n");
    people_destroy(person);
    return;
  }

  fprintf(txt, "- CPF: %s\n", people_get_cpf(person));
  fprintf(txt, "- Nome: %s %s\n", people_get_name(person), people_get_surname(person));
  fprintf(txt, "- Sexo: %c\n", people_get_gender(person));
  fprintf(txt, "- Nascimento: %s\n", people_get_date_of_birth(person));
  fprintf(txt, "- Endereço do despejo: %s/%c/%d (%s)\n", ads_get_cep(ads), ads_get_face(ads), ads_get_number(ads), ads_get_complement(ads));

  block_t *block = exh_get(blocks, ads_get_cep(ads));
  if (block != NULL) {
    point_t *pos = calc_address_pos(ads, block);

    circle_t *dot = circle_init(0, point_get_x(pos), point_get_y(pos), 5.0, "#000000", "#000000");
    llist_insertat_start(added_elements, shape_as_node(shape_init(CIRCLE, dot)));

    point_destroy(pos);
    block_destroy(block);
  }

  ads_destroy(ads);
  people_destroy(person);
}

void qry_processing(const char *qrypath, const char *txtpath, svg_t *svg, exhash_t *people, exhash_t *blocks, exhash_t *addresses) {
  FILE *qry = fopen(qrypath, "r");
  if (qry == NULL) {
    printf("Erro na leitura do arquivo .qry.\n");
    exit(1);
  }

  FILE *txt = fopen(txtpath, "w");
  if (txt == NULL) {
    printf("Erro na criação do arquivo .txt.\n");
    fclose(qry);
    exit(1);
  }

  llist_t *added_elements = llist_init();

  char buf[256];
  while (fgets(buf, 256, qry)) {
    if (strncmp(buf, "rq", 2) == 0) {
      char cep[16];

      sscanf(buf, "%*s %s", cep);
      command_rq(cep, blocks, addresses, people, txt, added_elements);
    } else if (strncmp(buf, "pq", 2) == 0) {
      char cep[16];

      sscanf(buf, "%*s %s", cep);
      command_pq(cep, blocks, addresses, txt, added_elements);
    } else if (strncmp(buf, "censo", 5) == 0) {
      command_censo(people, addresses, txt);
    } else if (strncmp(buf, "h?", 2) == 0) {
      char cpf[16];

      sscanf(buf, "%*s %s", cpf);
      command_h(cpf, people, addresses, txt);
    } else if (strncmp(buf, "nasc", 4) == 0) {
      char cpf[16], name[32], surname[32], dob[11];
      char gender;

      sscanf(buf, "%*s %s %s %s %c %s", cpf, name, surname, &gender, dob);
      command_nasc(cpf, name, surname, gender, dob, people, txt);
    } else if (strncmp(buf, "rip", 3) == 0) {
      char cpf[16];

      sscanf(buf, "%*s %s", cpf);
      command_rip(cpf, people, addresses, blocks, txt, added_elements);
    } else if (strncmp(buf, "dspj", 4) == 0) {
      char cpf[16];

      sscanf(buf, "%*s %s", cpf);
      command_dspj(cpf, people, addresses, blocks, txt, added_elements);
    } else if (strncmp(buf, "mud", 3) == 0) {
      char cpf[16], cep[16], complement[32];
      char face;
      uint16_t num;

      sscanf(buf, "%*s %s %s %c %" SCNu16 " %s", cpf, cep, &face, &num, complement);
      command_mud(cpf, cep, face, num, complement, people, addresses, blocks, txt, added_elements);
    }
  }

  svg_write_blocks(svg, blocks);
  size_t llen = llist_get_length(added_elements);
  for (size_t i = 0; i < llen; i++) {
    shape_t *shape = node_getvalue(llist_getat_index(added_elements, i));
    switch (shape_get_type(shape)) {
      case TEXT:
        svg_write_text(svg, shape_as_text(shape));
        break;
      case RECTANGLE:
        svg_write_rectangle(svg, shape_as_rectangle(shape));
        break;
      case CIRCLE:
        svg_write_circle(svg, shape_as_circle(shape));
        break;
    }
  }

  llist_destroy(added_elements);

  fclose(qry);
  fclose(txt);
}
