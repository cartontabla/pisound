/* param_slots_20.h
 * Se usa con:
 *   #define PARAM_SLOTS_DEFINE_SETTERS
 *   #define PARAM_SLOTS_DEFINE_TABLE
 */

#ifdef PARAM_SLOTS_DEFINE_SETTERS

/* --- Setters P1..P20 --- */
#ifdef PARAM_P1_LABEL
  #if !defined(PARAM_P1_LVALUE) && !defined(PARAM_P1_MEMBER)
    #error "PARAM_P1_LABEL definido pero falta PARAM_P1_LVALUE o PARAM_P1_MEMBER"
  #endif
  static inline void set_p1(float v){
    #if defined(PARAM_P1_LVALUE)
      PARAM_P1_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P1_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P2_LABEL
  #if !defined(PARAM_P2_LVALUE) && !defined(PARAM_P2_MEMBER)
    #error "PARAM_P2_LABEL definido pero falta PARAM_P2_LVALUE o PARAM_P2_MEMBER"
  #endif
  static inline void set_p2(float v){
    #if defined(PARAM_P2_LVALUE)
      PARAM_P2_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P2_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P3_LABEL
  #if !defined(PARAM_P3_LVALUE) && !defined(PARAM_P3_MEMBER)
    #error "PARAM_P3_LABEL definido pero falta PARAM_P3_LVALUE o PARAM_P3_MEMBER"
  #endif
  static inline void set_p3(float v){
    #if defined(PARAM_P3_LVALUE)
      PARAM_P3_LVALUE = (double)v;   /* aquí Mode_rt (uint8_T) es seguro */
    #else
      PARAM_STRUCT_VAR.PARAM_P3_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P4_LABEL
  #if !defined(PARAM_P4_LVALUE) && !defined(PARAM_P4_MEMBER)
    #error "PARAM_P4_LABEL definido pero falta PARAM_P4_LVALUE o PARAM_P4_MEMBER"
  #endif
  static inline void set_p4(float v){
    #if defined(PARAM_P4_LVALUE)
      PARAM_P4_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P4_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P5_LABEL
  #if !defined(PARAM_P5_LVALUE) && !defined(PARAM_P5_MEMBER)
    #error "PARAM_P5_LABEL definido pero falta PARAM_P5_LVALUE o PARAM_P5_MEMBER"
  #endif
  static inline void set_p5(float v){
    #if defined(PARAM_P5_LVALUE)
      PARAM_P5_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P5_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P6_LABEL
  #if !defined(PARAM_P6_LVALUE) && !defined(PARAM_P6_MEMBER)
    #error "PARAM_P6_LABEL definido pero falta PARAM_P6_LVALUE o PARAM_P6_MEMBER"
  #endif
  static inline void set_p6(float v){
    #if defined(PARAM_P6_LVALUE)
      PARAM_P6_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P6_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P7_LABEL
  #if !defined(PARAM_P7_LVALUE) && !defined(PARAM_P7_MEMBER)
    #error "PARAM_P7_LABEL definido pero falta PARAM_P7_LVALUE o PARAM_P7_MEMBER"
  #endif
  static inline void set_p7(float v){
    #if defined(PARAM_P7_LVALUE)
      PARAM_P7_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P7_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P8_LABEL
  #if !defined(PARAM_P8_LVALUE) && !defined(PARAM_P8_MEMBER)
    #error "PARAM_P8_LABEL definido pero falta PARAM_P8_LVALUE o PARAM_P8_MEMBER"
  #endif
  static inline void set_p8(float v){
    #if defined(PARAM_P8_LVALUE)
      PARAM_P8_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P8_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P9_LABEL
  #if !defined(PARAM_P9_LVALUE) && !defined(PARAM_P9_MEMBER)
    #error "PARAM_P9_LABEL definido pero falta PARAM_P9_LVALUE o PARAM_P9_MEMBER"
  #endif
  static inline void set_p9(float v){
    #if defined(PARAM_P9_LVALUE)
      PARAM_P9_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P9_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P10_LABEL
  #if !defined(PARAM_P10_LVALUE) && !defined(PARAM_P10_MEMBER)
    #error "PARAM_P10_LABEL definido pero falta PARAM_P10_LVALUE o PARAM_P10_MEMBER"
  #endif
  static inline void set_p10(float v){
    #if defined(PARAM_P10_LVALUE)
      PARAM_P10_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P10_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P11_LABEL
  #if !defined(PARAM_P11_LVALUE) && !defined(PARAM_P11_MEMBER)
    #error "PARAM_P11_LABEL definido pero falta PARAM_P11_LVALUE o PARAM_P11_MEMBER"
  #endif
  static inline void set_p11(float v){
    #if defined(PARAM_P11_LVALUE)
      PARAM_P11_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P11_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P12_LABEL
  #if !defined(PARAM_P12_LVALUE) && !defined(PARAM_P12_MEMBER)
    #error "PARAM_P12_LABEL definido pero falta PARAM_P12_LVALUE o PARAM_P12_MEMBER"
  #endif
  static inline void set_p12(float v){
    #if defined(PARAM_P12_LVALUE)
      PARAM_P12_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P12_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P13_LABEL
  #if !defined(PARAM_P13_LVALUE) && !defined(PARAM_P13_MEMBER)
    #error "PARAM_P13_LABEL definido pero falta PARAM_P13_LVALUE o PARAM_P13_MEMBER"
  #endif
  static inline void set_p13(float v){
    #if defined(PARAM_P13_LVALUE)
      PARAM_P13_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P13_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P14_LABEL
  #if !defined(PARAM_P14_LVALUE) && !defined(PARAM_P14_MEMBER)
    #error "PARAM_P14_LABEL definido pero falta PARAM_P14_LVALUE o PARAM_P14_MEMBER"
  #endif
  static inline void set_p14(float v){
    #if defined(PARAM_P14_LVALUE)
      PARAM_P14_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P14_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P15_LABEL
  #if !defined(PARAM_P15_LVALUE) && !defined(PARAM_P15_MEMBER)
    #error "PARAM_P15_LABEL definido pero falta PARAM_P15_LVALUE o PARAM_P15_MEMBER"
  #endif
  static inline void set_p15(float v){
    #if defined(PARAM_P15_LVALUE)
      PARAM_P15_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P15_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P16_LABEL
  #if !defined(PARAM_P16_LVALUE) && !defined(PARAM_P16_MEMBER)
    #error "PARAM_P16_LABEL definido pero falta PARAM_P16_LVALUE o PARAM_P16_MEMBER"
  #endif
  static inline void set_p16(float v){
    #if defined(PARAM_P16_LVALUE)
      PARAM_P16_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P16_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P17_LABEL
  #if !defined(PARAM_P17_LVALUE) && !defined(PARAM_P17_MEMBER)
    #error "PARAM_P17_LABEL definido pero falta PARAM_P17_LVALUE o PARAM_P17_MEMBER"
  #endif
  static inline void set_p17(float v){
    #if defined(PARAM_P17_LVALUE)
      PARAM_P17_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P17_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P18_LABEL
  #if !defined(PARAM_P18_LVALUE) && !defined(PARAM_P18_MEMBER)
    #error "PARAM_P18_LABEL definido pero falta PARAM_P18_LVALUE o PARAM_P18_MEMBER"
  #endif
  static inline void set_p18(float v){
    #if defined(PARAM_P18_LVALUE)
      PARAM_P18_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P18_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P19_LABEL
  #if !defined(PARAM_P19_LVALUE) && !defined(PARAM_P19_MEMBER)
    #error "PARAM_P19_LABEL definido pero falta PARAM_P19_LVALUE o PARAM_P19_MEMBER"
  #endif
  static inline void set_p19(float v){
    #if defined(PARAM_P19_LVALUE)
      PARAM_P19_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P19_MEMBER = (double)v;
    #endif
  }
#endif

#ifdef PARAM_P20_LABEL
  #if !defined(PARAM_P20_LVALUE) && !defined(PARAM_P20_MEMBER)
    #error "PARAM_P20_LABEL definido pero falta PARAM_P20_LVALUE o PARAM_P20_MEMBER"
  #endif
  static inline void set_p20(float v){
    #if defined(PARAM_P20_LVALUE)
      PARAM_P20_LVALUE = (double)v;
    #else
      PARAM_STRUCT_VAR.PARAM_P20_MEMBER = (double)v;
    #endif
  }
#endif

#endif /* PARAM_SLOTS_DEFINE_SETTERS */


#ifdef PARAM_SLOTS_DEFINE_TABLE

/* --- Table entries P1..P20 --- */
#ifdef PARAM_P1_LABEL
  { PARAM_P1_LABEL, set_p1 },
#endif
#ifdef PARAM_P2_LABEL
  { PARAM_P2_LABEL, set_p2 },
#endif
#ifdef PARAM_P3_LABEL
  { PARAM_P3_LABEL, set_p3 },
#endif
#ifdef PARAM_P4_LABEL
  { PARAM_P4_LABEL, set_p4 },
#endif
#ifdef PARAM_P5_LABEL
  { PARAM_P5_LABEL, set_p5 },
#endif
#ifdef PARAM_P6_LABEL
  { PARAM_P6_LABEL, set_p6 },
#endif
#ifdef PARAM_P7_LABEL
  { PARAM_P7_LABEL, set_p7 },
#endif
#ifdef PARAM_P8_LABEL
  { PARAM_P8_LABEL, set_p8 },
#endif
#ifdef PARAM_P9_LABEL
  { PARAM_P9_LABEL, set_p9 },
#endif
#ifdef PARAM_P10_LABEL
  { PARAM_P10_LABEL, set_p10 },
#endif
#ifdef PARAM_P11_LABEL
  { PARAM_P11_LABEL, set_p11 },
#endif
#ifdef PARAM_P12_LABEL
  { PARAM_P12_LABEL, set_p12 },
#endif
#ifdef PARAM_P13_LABEL
  { PARAM_P13_LABEL, set_p13 },
#endif
#ifdef PARAM_P14_LABEL
  { PARAM_P14_LABEL, set_p14 },
#endif
#ifdef PARAM_P15_LABEL
  { PARAM_P15_LABEL, set_p15 },
#endif
#ifdef PARAM_P16_LABEL
  { PARAM_P16_LABEL, set_p16 },
#endif
#ifdef PARAM_P17_LABEL
  { PARAM_P17_LABEL, set_p17 },
#endif
#ifdef PARAM_P18_LABEL
  { PARAM_P18_LABEL, set_p18 },
#endif
#ifdef PARAM_P19_LABEL
  { PARAM_P19_LABEL, set_p19 },
#endif
#ifdef PARAM_P20_LABEL
  { PARAM_P20_LABEL, set_p20 },
#endif

#endif /* PARAM_SLOTS_DEFINE_TABLE */