/* mon_slots_20.h
 * Se usa con:
 *   #define MON_SLOTS_DEFINE_GETTERS
 *   #define MON_SLOTS_DEFINE_TABLE
 */

#ifdef MON_SLOTS_DEFINE_GETTERS

/* --- Getters M1..M20 --- */
#ifdef MON_M1_LABEL
  #if !defined(MON_M1_VALUE)
    #error "MON_M1_LABEL definido pero falta MON_M1_VALUE"
  #endif
  static inline double get_m1(void){
    return (double)(MON_M1_VALUE);
  }
#endif

#ifdef MON_M2_LABEL
  #if !defined(MON_M2_VALUE)
    #error "MON_M2_LABEL definido pero falta MON_M2_VALUE"
  #endif
  static inline double get_m2(void){
    return (double)(MON_M2_VALUE);
  }
#endif

#ifdef MON_M3_LABEL
  #if !defined(MON_M3_VALUE)
    #error "MON_M3_LABEL definido pero falta MON_M3_VALUE"
  #endif
  static inline double get_m3(void){
    return (double)(MON_M3_VALUE);
  }
#endif

#ifdef MON_M4_LABEL
  #if !defined(MON_M4_VALUE)
    #error "MON_M4_LABEL definido pero falta MON_M4_VALUE"
  #endif
  static inline double get_m4(void){
    return (double)(MON_M4_VALUE);
  }
#endif

#ifdef MON_M5_LABEL
  #if !defined(MON_M5_VALUE)
    #error "MON_M5_LABEL definido pero falta MON_M5_VALUE"
  #endif
  static inline double get_m5(void){
    return (double)(MON_M5_VALUE);
  }
#endif

#ifdef MON_M6_LABEL
  #if !defined(MON_M6_VALUE)
    #error "MON_M6_LABEL definido pero falta MON_M6_VALUE"
  #endif
  static inline double get_m6(void){
    return (double)(MON_M6_VALUE);
  }
#endif

#ifdef MON_M7_LABEL
  #if !defined(MON_M7_VALUE)
    #error "MON_M7_LABEL definido pero falta MON_M7_VALUE"
  #endif
  static inline double get_m7(void){
    return (double)(MON_M7_VALUE);
  }
#endif

#ifdef MON_M8_LABEL
  #if !defined(MON_M8_VALUE)
    #error "MON_M8_LABEL definido pero falta MON_M8_VALUE"
  #endif
  static inline double get_m8(void){
    return (double)(MON_M8_VALUE);
  }
#endif

#ifdef MON_M9_LABEL
  #if !defined(MON_M9_VALUE)
    #error "MON_M9_LABEL definido pero falta MON_M9_VALUE"
  #endif
  static inline double get_m9(void){
    return (double)(MON_M9_VALUE);
  }
#endif

#ifdef MON_M10_LABEL
  #if !defined(MON_M10_VALUE)
    #error "MON_M10_LABEL definido pero falta MON_M10_VALUE"
  #endif
  static inline double get_m10(void){
    return (double)(MON_M10_VALUE);
  }
#endif

#ifdef MON_M11_LABEL
  #if !defined(MON_M11_VALUE)
    #error "MON_M11_LABEL definido pero falta MON_M11_VALUE"
  #endif
  static inline double get_m11(void){
    return (double)(MON_M11_VALUE);
  }
#endif

#ifdef MON_M12_LABEL
  #if !defined(MON_M12_VALUE)
    #error "MON_M12_LABEL definido pero falta MON_M12_VALUE"
  #endif
  static inline double get_m12(void){
    return (double)(MON_M12_VALUE);
  }
#endif

#ifdef MON_M13_LABEL
  #if !defined(MON_M13_VALUE)
    #error "MON_M13_LABEL definido pero falta MON_M13_VALUE"
  #endif
  static inline double get_m13(void){
    return (double)(MON_M13_VALUE);
  }
#endif

#ifdef MON_M14_LABEL
  #if !defined(MON_M14_VALUE)
    #error "MON_M14_LABEL definido pero falta MON_M14_VALUE"
  #endif
  static inline double get_m14(void){
    return (double)(MON_M14_VALUE);
  }
#endif

#ifdef MON_M15_LABEL
  #if !defined(MON_M15_VALUE)
    #error "MON_M15_LABEL definido pero falta MON_M15_VALUE"
  #endif
  static inline double get_m15(void){
    return (double)(MON_M15_VALUE);
  }
#endif

#ifdef MON_M16_LABEL
  #if !defined(MON_M16_VALUE)
    #error "MON_M16_LABEL definido pero falta MON_M16_VALUE"
  #endif
  static inline double get_m16(void){
    return (double)(MON_M16_VALUE);
  }
#endif

#ifdef MON_M17_LABEL
  #if !defined(MON_M17_VALUE)
    #error "MON_M17_LABEL definido pero falta MON_M17_VALUE"
  #endif
  static inline double get_m17(void){
    return (double)(MON_M17_VALUE);
  }
#endif

#ifdef MON_M18_LABEL
  #if !defined(MON_M18_VALUE)
    #error "MON_M18_LABEL definido pero falta MON_M18_VALUE"
  #endif
  static inline double get_m18(void){
    return (double)(MON_M18_VALUE);
  }
#endif

#ifdef MON_M19_LABEL
  #if !defined(MON_M19_VALUE)
    #error "MON_M19_LABEL definido pero falta MON_M19_VALUE"
  #endif
  static inline double get_m19(void){
    return (double)(MON_M19_VALUE);
  }
#endif

#ifdef MON_M20_LABEL
  #if !defined(MON_M20_VALUE)
    #error "MON_M20_LABEL definido pero falta MON_M20_VALUE"
  #endif
  static inline double get_m20(void){
    return (double)(MON_M20_VALUE);
  }
#endif

#endif /* MON_SLOTS_DEFINE_GETTERS */


#ifdef MON_SLOTS_DEFINE_TABLE

/* --- Table entries M1..M20 --- */
#ifdef MON_M1_LABEL
  {
    MON_M1_LABEL,
    MON_M1_OSC,
    get_m1,
    #ifdef MON_M1_DECIM
      MON_M1_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M1_THRESHOLD
      MON_M1_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M2_LABEL
  {
    MON_M2_LABEL,
    MON_M2_OSC,
    get_m2,
    #ifdef MON_M2_DECIM
      MON_M2_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M2_THRESHOLD
      MON_M2_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M3_LABEL
  {
    MON_M3_LABEL,
    MON_M3_OSC,
    get_m3,
    #ifdef MON_M3_DECIM
      MON_M3_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M3_THRESHOLD
      MON_M3_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M4_LABEL
  {
    MON_M4_LABEL,
    MON_M4_OSC,
    get_m4,
    #ifdef MON_M4_DECIM
      MON_M4_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M4_THRESHOLD
      MON_M4_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M5_LABEL
  {
    MON_M5_LABEL,
    MON_M5_OSC,
    get_m5,
    #ifdef MON_M5_DECIM
      MON_M5_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M5_THRESHOLD
      MON_M5_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M6_LABEL
  {
    MON_M6_LABEL,
    MON_M6_OSC,
    get_m6,
    #ifdef MON_M6_DECIM
      MON_M6_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M6_THRESHOLD
      MON_M6_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M7_LABEL
  {
    MON_M7_LABEL,
    MON_M7_OSC,
    get_m7,
    #ifdef MON_M7_DECIM
      MON_M7_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M7_THRESHOLD
      MON_M7_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M8_LABEL
  {
    MON_M8_LABEL,
    MON_M8_OSC,
    get_m8,
    #ifdef MON_M8_DECIM
      MON_M8_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M8_THRESHOLD
      MON_M8_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M9_LABEL
  {
    MON_M9_LABEL,
    MON_M9_OSC,
    get_m9,
    #ifdef MON_M9_DECIM
      MON_M9_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M9_THRESHOLD
      MON_M9_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M10_LABEL
  {
    MON_M10_LABEL,
    MON_M10_OSC,
    get_m10,
    #ifdef MON_M10_DECIM
      MON_M10_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M10_THRESHOLD
      MON_M10_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M11_LABEL
  {
    MON_M11_LABEL,
    MON_M11_OSC,
    get_m11,
    #ifdef MON_M11_DECIM
      MON_M11_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M11_THRESHOLD
      MON_M11_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M12_LABEL
  {
    MON_M12_LABEL,
    MON_M12_OSC,
    get_m12,
    #ifdef MON_M12_DECIM
      MON_M12_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M12_THRESHOLD
      MON_M12_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M13_LABEL
  {
    MON_M13_LABEL,
    MON_M13_OSC,
    get_m13,
    #ifdef MON_M13_DECIM
      MON_M13_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M13_THRESHOLD
      MON_M13_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M14_LABEL
  {
    MON_M14_LABEL,
    MON_M14_OSC,
    get_m14,
    #ifdef MON_M14_DECIM
      MON_M14_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M14_THRESHOLD
      MON_M14_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M15_LABEL
  {
    MON_M15_LABEL,
    MON_M15_OSC,
    get_m15,
    #ifdef MON_M15_DECIM
      MON_M15_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M15_THRESHOLD
      MON_M15_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M16_LABEL
  {
    MON_M16_LABEL,
    MON_M16_OSC,
    get_m16,
    #ifdef MON_M16_DECIM
      MON_M16_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M16_THRESHOLD
      MON_M16_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M17_LABEL
  {
    MON_M17_LABEL,
    MON_M17_OSC,
    get_m17,
    #ifdef MON_M17_DECIM
      MON_M17_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M17_THRESHOLD
      MON_M17_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M18_LABEL
  {
    MON_M18_LABEL,
    MON_M18_OSC,
    get_m18,
    #ifdef MON_M18_DECIM
      MON_M18_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M18_THRESHOLD
      MON_M18_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M19_LABEL
  {
    MON_M19_LABEL,
    MON_M19_OSC,
    get_m19,
    #ifdef MON_M19_DECIM
      MON_M19_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M19_THRESHOLD
      MON_M19_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#ifdef MON_M20_LABEL
  {
    MON_M20_LABEL,
    MON_M20_OSC,
    get_m20,
    #ifdef MON_M20_DECIM
      MON_M20_DECIM,
    #else
      1,
    #endif
    #ifdef MON_M20_THRESHOLD
      MON_M20_THRESHOLD,
    #else
      0.0,
    #endif
    0, 0.0, 1
  },
#endif

#endif /* MON_SLOTS_DEFINE_TABLE */