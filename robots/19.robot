{
   "backend" : "xl-320",
   "parameters" : {
      "L" : {
         "max" : "350",
         "min" : "20",
         "val" : "130"
      },
      "L1" : {
         "max" : "100",
         "min" : "20",
         "val" : "45"
      },
      "L2" : {
         "max" : "100",
         "min" : "20",
         "val" : "45"
      },
      "L3" : {
         "max" : "100",
         "min" : "20",
         "val" : "30"
      },
      "W" : {
         "max" : "200",
         "min" : "20",
         "val" : "55"
      }
   },
   "tips" : 6,
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : -1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "1" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "2" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : -1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "4" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : -1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         },
         "5" : {
            "component" : {
               "anchors" : {
                  "1" : {
                     "component" : {
                        "anchors" : {
                           "1" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "bottom_leg",
                                 "parameters" : {
                                    "FixationAngle" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "0"
                                    },
                                    "SizeA" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "SizeB" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeBottom" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "10"
                                    },
                                    "SizeC" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "20"
                                    },
                                    "SizeTop" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 3.1415927410125732,
                              "minimum" : 0,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "arm_to_arm",
                        "parameters" : {
                           "Distance" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : 0,
                     "remote" : 0,
                     "zero" : 0
                  }
               },
               "component" : "arm_to_side",
               "parameters" : {
                  "Distance" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "FirstPart" : {
                     "max" : "",
                     "min" : "",
                     "val" : "17"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 1.5707963705062866,
            "remote" : 0,
            "zero" : 0
         }
      },
      "component" : "square_body_side",
      "parameters" : {
         "Legs" : {
            "max" : "",
            "min" : "",
            "val" : "6"
         },
         "LegsSpacing" : {
            "max" : "",
            "min" : "",
            "val" : "$L"
         },
         "Thickness" : {
            "max" : "",
            "min" : "",
            "val" : "2.2"
         },
         "Width" : {
            "max" : "",
            "min" : "",
            "val" : "$W"
         }
      }
   }
}
