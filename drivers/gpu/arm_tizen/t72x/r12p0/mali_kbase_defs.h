/*
 *
 * (C) COPYRIGHT 2011-2016 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */





/**
 * @file mali_kbase_defs.h
 *
 * Defintions (types, defines, etcs) common to Kbase. They are placed here to
 * allow the hierarchy of header files to work.
 */

#ifndef _KBASE_DEFS_H_
#define _KBASE_DEFS_H_

#include <mali_kbase_config.h>
#include <mali_base_hwconfig_features.h>
#include <mali_base_hwconfig_issues.h>
#include <mali_kbase_mem_lowlevel.h>
#include <mali_kbase_mmu_hw.h>
#include <mali_kbase_mmu_mode.h>
#include <mali_kbase_instr.h>

#include <linux/atomic.h>
#include <linux/mempool.h>
#include <linux/slab.h>
#include <linux/file.h>

#ifdef CONFIG_MALI_FPGA_BUS_LOGGER
#include <linux/bus_logger.h>
#endif


#ifdef CONFIG_KDS
#include <linux/kds.h>
#endif				/* CONFIG_KDS */

#ifdef CONFIG_SYNC
#include "sync.h"
#endif				/* CONFIG_SYNC */

#include "mali_kbase_dma_fence.h"

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#endif				/* CONFIG_DEBUG_FS */

#ifdef CONFIG_PM_DEVFREQ
#include <linux/devfreq.h>
#endif /* CONFIG_DEVFREQ */

#include <linux/clk.h>
#include <linux/regulator/consumer.h>
/* MALI_SEC_INTEGRATION */
#include <platform/exynos/gpu_integration_defs.h>
#if defined(CONFIG_PM_RUNTIME) || \
	(defined(CONFIG_PM) && LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0))
#define KBASE_PM_RUNTIME 1
#endif

/** Enable SW tracing when set */
/* MALI_SEC_INTEGRATION */
#if defined(CONFIG_MALI_MIDGARD_ENABLE_TRACE) || defined(CONFIG_MALI_EXYNOS_TRACE)
#define KBASE_TRACE_ENABLE 1
#endif

#ifndef KBASE_TRACE_ENABLE
#ifdef CONFIG_MALI_DEBUG
#define KBASE_TRACE_ENABLE 1
#else
#define KBASE_TRACE_ENABLE 0
#endif				/* CONFIG_MALI_DEBUG */
#endif				/* KBASE_TRACE_ENABLE */

/** Dump Job slot trace on error (only active if KBASE_TRACE_ENABLE != 0) */
#define KBASE_TRACE_DUMP_ON_JOB_SLOT_ERROR 1

/**
 * Number of milliseconds before resetting the GPU when a job cannot be "zapped" from the hardware.
 * Note that the time is actually ZAP_TIMEOUT+SOFT_STOP_RESET_TIMEOUT between the context zap starting and the GPU
 * actually being reset to give other contexts time for their jobs to be soft-stopped and removed from the hardware
 * before resetting.
 */
#define ZAP_TIMEOUT             1000

/** Number of milliseconds before we time out on a GPU soft/hard reset */
#define RESET_TIMEOUT           500

/**
 * Prevent soft-stops from occuring in scheduling situations
 *
 * This is not due to HW issues, but when scheduling is desired to be more predictable.
 *
 * Therefore, soft stop may still be disabled due to HW issues.
 *
 * @note Soft stop will still be used for non-scheduling purposes e.g. when terminating a context.
 *
 * @note if not in use, define this value to 0 instead of \#undef'ing it
 */
#define KBASE_DISABLE_SCHEDULING_SOFT_STOPS 0

/**
 * Prevent hard-stops from occuring in scheduling situations
 *
 * This is not due to HW issues, but when scheduling is desired to be more predictable.
 *
 * @note Hard stop will still be used for non-scheduling purposes e.g. when terminating a context.
 *
 * @note if not in use, define this value to 0 instead of \#undef'ing it
 */
#define KBASE_DISABLE_SCHEDULING_HARD_STOPS 0

/**
 * The maximum number of Job Slots to support in the Hardware.
 *
 * You can optimize this down if your target devices will only ever support a
 * small number of job slots.
 */
#define BASE_JM_MAX_NR_SLOTS        3

/**
 * The maximum number of Address Spaces to support in the Hardware.
 *
 * You can optimize this down if your target devices will only ever support a
 * small number of Address Spaces
 */
#define BASE_MAX_NR_AS              16

/* mmu */
#define MIDGARD_MMU_VA_BITS 48

#if MIDGARD_MMU_VA_BITS > 39
#define MIDGARD_MMU_TOPLEVEL    0
#else
#define MIDGARD_MMU_TOPLEVEL    1
#endif

#define MIDGARD_MMU_BOTTOMLEVEL 3

#define GROWABLE_FLAGS_REQUIRED (KBASE_REG_PF_GROW | KBASE_REG_GPU_WR)

/** setting in kbase_context::as_nr that indicates it's invalid */
#define KBASEP_AS_NR_INVALID     (-1)

#define KBASE_LOCK_REGION_MAX_SIZE (63)
#define KBASE_LOCK_REGION_MIN_SIZE (11)

/* MALI_SEC_INTEGRATION */
#ifdef CONFIG_MALI_EXYNOS_TRACE
#define KBASE_TRACE_SIZE_LOG2 10	/* 1024 entries */
#else
#define KBASE_TRACE_SIZE_LOG2 8	/* 256 entries */
#endif
#define KBASE_TRACE_SIZE (1 << KBASE_TRACE_SIZE_LOG2)
#define KBASE_TRACE_MASK ((1 << KBASE_TRACE_SIZE_LOG2)-1)

#include "mali_kbase_js_defs.h"
#include "mali_kbase_hwaccess_defs.h"

#define KBASEP_FORCE_REPLAY_DISABLED 0

/* Maximum force replay limit when randomization is enabled */
#define KBASEP_FORCE_REPLAY_RANDOM_LIMIT 16

/** Atom has been previously soft-stoppped */
#define KBASE_KATOM_FLAG_BEEN_SOFT_STOPPPED (1<<1)
/** Atom has been previously retried to execute */
#define KBASE_KATOM_FLAGS_RERUN (1<<2)
#define KBASE_KATOM_FLAGS_JOBCHAIN (1<<3)
/** Atom has been previously hard-stopped. */
#define KBASE_KATOM_FLAG_BEEN_HARD_STOPPED (1<<4)
/** Atom has caused us to enter disjoint state */
#define KBASE_KATOM_FLAG_IN_DISJOINT (1<<5)
/* Atom has fail dependency on same-slot dependency */
#define KBASE_KATOM_FLAG_FAIL_PREV (1<<6)
/* Atom blocked on cross-slot dependency */
#define KBASE_KATOM_FLAG_X_DEP_BLOCKED (1<<7)
/* Atom has fail dependency on cross-slot dependency */
#define KBASE_KATOM_FLAG_FAIL_BLOCKER (1<<8)
/* Atom has been submitted to JSCTX ringbuffers */
#define KBASE_KATOM_FLAG_JSCTX_RB_SUBMITTED (1<<9)
/* Atom is currently holding a context reference */
#define KBASE_KATOM_FLAG_HOLDING_CTX_REF (1<<10)
/* Atom requires GPU to be in secure mode */
#define KBASE_KATOM_FLAG_SECURE (1<<11)
/* Atom has been stored in linked list */
#define KBASE_KATOM_FLAG_JSCTX_IN_LL (1<<12)

/* SW related flags about types of JS_COMMAND action
 * NOTE: These must be masked off by JS_COMMAND_MASK */

/** This command causes a disjoint event */
#define JS_COMMAND_SW_CAUSES_DISJOINT 0x100

/** Bitmask of all SW related flags */
#define JS_COMMAND_SW_BITS  (JS_COMMAND_SW_CAUSES_DISJOINT)

#if (JS_COMMAND_SW_BITS & JS_COMMAND_MASK)
#error JS_COMMAND_SW_BITS not masked off by JS_COMMAND_MASK. Must update JS_COMMAND_SW_<..> bitmasks
#endif

/** Soft-stop command that causes a Disjoint event. This of course isn't
 *  entirely masked off by JS_COMMAND_MASK */
#define JS_COMMAND_SOFT_STOP_WITH_SW_DISJOINT \
		(JS_COMMAND_SW_CAUSES_DISJOINT | JS_COMMAND_SOFT_STOP)

#define KBASEP_ATOM_ID_INVALID BASE_JD_ATOM_COUNT

#ifdef CONFIG_DEBUG_FS
struct base_job_fault_event {

	u32 event_code;
	struct kbase_jd_atom *katom;
	struct work_struct job_fault_work;
	struct list_head head;
	int reg_offset;
};

#endif

struct kbase_jd_atom_dependency {
	struct kbase_jd_atom *atom;
	u8 dep_type;
};

/**
 * @brief The function retrieves a read-only reference to the atom field from
 * the  kbase_jd_atom_dependency structure
 *
 * @param[in] dep kbase jd atom dependency.
 *
 * @return readonly reference to dependent ATOM.
 */
static inline const struct kbase_jd_atom *const kbase_jd_katom_dep_atom(const struct kbase_jd_atom_dependency *dep)
{
	LOCAL_ASSERT(dep != NULL);

	return (const struct kbase_jd_atom * const)(dep->atom);
}

/**
 * @brief The function retrieves a read-only reference to the dependency type field from
 * the  kbase_jd_atom_dependency structure
 *
 * @param[in] dep kbase jd atom dependency.
 *
 * @return A dependency type value.
 */
static inline const u8 kbase_jd_katom_dep_type(const struct kbase_jd_atom_dependency *dep)
{
	LOCAL_ASSERT(dep != NULL);

	return dep->dep_type;
}

/**
 * @brief Setter macro for dep_atom array entry in kbase_jd_atom
 *
 * @param[in] dep    The kbase jd atom dependency.
 * @param[in] a      The ATOM to be set as a dependency.
 * @param     type   The ATOM dependency type to be set.
 *
 */
static inline void kbase_jd_katom_dep_set(const struct kbase_jd_atom_dependency *const_dep,
		struct kbase_jd_atom *a, u8 type)
{
	struct kbase_jd_atom_dependency *dep;

	LOCAL_ASSERT(const_dep != NULL);

	dep = (struct kbase_jd_atom_dependency *)const_dep;

	dep->atom = a;
	dep->dep_type = type;
}

/**
 * @brief Setter macro for dep_atom array entry in kbase_jd_atom
 *
 * @param[in] dep    The kbase jd atom dependency to be cleared.
 *
 */
static inline void kbase_jd_katom_dep_clear(const struct kbase_jd_atom_dependency *const_dep)
{
	struct kbase_jd_atom_dependency *dep;

	LOCAL_ASSERT(const_dep != NULL);

	dep = (struct kbase_jd_atom_dependency *)const_dep;

	dep->atom = NULL;
	dep->dep_type = BASE_JD_DEP_TYPE_INVALID;
}

enum kbase_atom_gpu_rb_state {
	/* Atom is not currently present in slot ringbuffer */
	KBASE_ATOM_GPU_RB_NOT_IN_SLOT_RB,
	/* Atom is in slot ringbuffer but is blocked on a previous atom */
	KBASE_ATOM_GPU_RB_WAITING_BLOCKED,
	/* Atom is in slot ringbuffer but is waiting for cores to become
	 * available */
	KBASE_ATOM_GPU_RB_WAITING_FOR_CORE_AVAILABLE,
	/* Atom is in slot ringbuffer but is blocked on affinity */
	KBASE_ATOM_GPU_RB_WAITING_AFFINITY,
	/* Atom is in slot ringbuffer but is waiting for secure mode switch */
	KBASE_ATOM_GPU_RB_WAITING_SECURE_MODE,
	/* Atom is in slot ringbuffer and ready to run */
	KBASE_ATOM_GPU_RB_READY,
	/* Atom is in slot ringbuffer and has been submitted to the GPU */
	KBASE_ATOM_GPU_RB_SUBMITTED,
	/* Atom must be returned to JS as soon as it reaches the head of the
	 * ringbuffer due to a previous failure */
	KBASE_ATOM_GPU_RB_RETURN_TO_JS
};

struct kbase_ext_res {
	u64 gpu_address;
	struct kbase_mem_phy_alloc *alloc;
};

struct kbase_jd_atom {
	struct work_struct work;
	ktime_t start_timestamp;
	u64 time_spent_us; /**< Total time spent on the GPU in microseconds */

	struct base_jd_udata udata;
	struct kbase_context *kctx;

	struct list_head dep_head[2];
	struct list_head dep_item[2];
	const struct kbase_jd_atom_dependency dep[2];

	u16 nr_extres;
	struct kbase_ext_res *extres;

	u32 device_nr;
	u64 affinity;
	u64 jc;
	enum kbase_atom_coreref_state coreref_state;
#ifdef CONFIG_KDS
	struct list_head node;
	struct kds_resource_set *kds_rset;
	bool kds_dep_satisfied;
#endif				/* CONFIG_KDS */
#ifdef CONFIG_SYNC
	struct sync_fence *fence;
	struct sync_fence_waiter sync_waiter;
/* MALI_SEC_INTEGRATION */
	spinlock_t fence_lock;
	struct mutex fence_mt;
	struct timer_list fence_timer;
#endif				/* CONFIG_SYNC */

#ifdef CONFIG_MALI_DMA_FENCE
	struct {
		/* This points to the dma-buf fence for this atom. If this is
		 * NULL then there is no fence for this atom and the other
		 * fields related to dma_fence may have invalid data.
		 *
		 * The context and seqno fields contain the details for this
		 * fence.
		 *
		 * This fence is signaled when the katom is completed,
		 * regardless of the event_code of the katom (signal also on
		 * failure).
		 */
		struct fence *fence;
		/* The dma-buf fence context number for this atom. A unique
		 * context number is allocated to each katom in the context on
		 * context creation.
		 */
		unsigned int context;
		/* The dma-buf fence sequence number for this atom. This is
		 * increased every time this katom uses dma-buf fence.
		 */
		atomic_t seqno;
		/* This contains a list of all callbacks set up to wait on
		 * other fences.  This atom must be held back from JS until all
		 * these callbacks have been called and dep_count have reached
		 * 0. The initial value of dep_count must be equal to the
		 * number of callbacks on this list.
		 *
		 * This list is protected by jctx.lock. Callbacks are added to
		 * this list when the atom is built and the wait are set up.
		 * All the callbacks then stay on the list until all callbacks
		 * have been called and the atom is queued, or cancelled, and
		 * then all callbacks are taken off the list and freed.
		 */
		struct list_head callbacks;
		/* Atomic counter of number of outstandind dma-buf fence
		 * dependencies for this atom. When dep_count reaches 0 the
		 * atom may be queued.
		 *
		 * The special value "-1" may only be set after the count
		 * reaches 0, while holding jctx.lock. This indicates that the
		 * atom has been handled, either queued in JS or cancelled.
		 *
		 * If anyone but the dma-fence worker sets this to -1 they must
		 * ensure that any potentially queued worker must have
		 * completed before allowing the atom to be marked as unused.
		 * This can be done by flushing the fence work queue:
		 * kctx->dma_fence.wq.
		 */
		atomic_t dep_count;
	} dma_fence;
#endif /* CONFIG_MALI_DMA_FENCE */

	/* Note: refer to kbasep_js_atom_retained_state, which will take a copy of some of the following members */
	enum base_jd_event_code event_code;
	base_jd_core_req core_req;	    /**< core requirements */
	/** Job Slot to retry submitting to if submission from IRQ handler failed
	 *
	 * NOTE: see if this can be unified into the another member e.g. the event */
	int retry_submit_on_slot;

	union kbasep_js_policy_job_info sched_info;
	/* JS atom priority with respect to other atoms on its kctx. */
	int sched_priority;

	int poking;		/* BASE_HW_ISSUE_8316 */

	wait_queue_head_t completed;
	enum kbase_jd_atom_state status;
#ifdef CONFIG_GPU_TRACEPOINTS
	int work_id;
#endif
	/* Assigned after atom is completed. Used to check whether PRLAM-10676 workaround should be applied */
	int slot_nr;

	u32 atom_flags;

	/* Number of times this atom has been retried. Used by replay soft job.
	 */
	int retry_count;

	enum kbase_atom_gpu_rb_state gpu_rb_state;

	u64 need_cache_flush_cores_retained;

	atomic_t blocked;

	/* Pointer to atom that this atom has cross-slot dependency on */
	struct kbase_jd_atom *x_pre_dep;
	/* Pointer to atom that has cross-slot dependency on this atom */
	struct kbase_jd_atom *x_post_dep;

	/* The GPU's flush count recorded at the time of submission, used for
	 * the cache flush optimisation */
	u32 flush_id;

	struct kbase_jd_atom_backend backend;
#ifdef CONFIG_DEBUG_FS
	struct base_job_fault_event fault_event;
#endif

#ifdef CONFIG_MALI_DMA_FENCE
	/*
	 * This queue attribute is added from r10p0 for JS not dma buf fence,
	 * so adds it for dma buf fence temporarily.
	 */
	struct list_head queue;
#endif

	struct kbase_va_region *jit_addr_reg;

	/* If non-zero, this indicates that the atom will fail with the set
	 * event_code when the atom is processed. */
	enum base_jd_event_code will_fail_event_code;
};

static inline bool kbase_jd_katom_is_secure(const struct kbase_jd_atom *katom)
{
	return (bool)(katom->atom_flags & KBASE_KATOM_FLAG_SECURE);
}

/*
 * Theory of operations:
 *
 * Atom objects are statically allocated within the context structure.
 *
 * Each atom is the head of two lists, one for the "left" set of dependencies, one for the "right" set.
 */

#define KBASE_JD_DEP_QUEUE_SIZE 256

struct kbase_jd_context {
	struct mutex lock;
	struct kbasep_js_kctx_info sched_info;
	struct kbase_jd_atom atoms[BASE_JD_ATOM_COUNT];

	/** Tracks all job-dispatch jobs.  This includes those not tracked by
	 * the scheduler: 'not ready to run' and 'dependency-only' jobs. */
	u32 job_nr;

	/** Waitq that reflects whether there are no jobs (including SW-only
	 * dependency jobs). This is set when no jobs are present on the ctx,
	 * and clear when there are jobs.
	 *
	 * @note: Job Dispatcher knows about more jobs than the Job Scheduler:
	 * the Job Scheduler is unaware of jobs that are blocked on dependencies,
	 * and SW-only dependency jobs.
	 *
	 * This waitq can be waited upon to find out when the context jobs are all
	 * done/cancelled (including those that might've been blocked on
	 * dependencies) - and so, whether it can be terminated. However, it should
	 * only be terminated once it is neither present in the policy-queue (see
	 * kbasep_js_policy_try_evict_ctx() ) nor the run-pool (see
	 * kbasep_js_kctx_info::ctx::is_scheduled).
	 *
	 * Since the waitq is only set under kbase_jd_context::lock,
	 * the waiter should also briefly obtain and drop kbase_jd_context::lock to
	 * guarentee that the setter has completed its work on the kbase_context
	 *
	 * This must be updated atomically with:
	 * - kbase_jd_context::job_nr */
	wait_queue_head_t zero_jobs_wait;

	/** Job Done workqueue. */
	struct workqueue_struct *job_done_wq;

	spinlock_t tb_lock;
	u32 *tb;
	size_t tb_wrap_offset;

#ifdef CONFIG_KDS
	struct kds_callback kds_cb;
#endif				/* CONFIG_KDS */
#ifdef CONFIG_GPU_TRACEPOINTS
	atomic_t work_id;
#endif
};

struct kbase_device_info {
	u32 features;
};

/** Poking state for BASE_HW_ISSUE_8316  */
enum {
	KBASE_AS_POKE_STATE_IN_FLIGHT     = 1<<0,
	KBASE_AS_POKE_STATE_KILLING_POKE  = 1<<1
};

/** Poking state for BASE_HW_ISSUE_8316  */
typedef u32 kbase_as_poke_state;

struct kbase_mmu_setup {
	u64	transtab;
	u64	memattr;
	u64	transcfg;
};

/**
 * Important: Our code makes assumptions that a struct kbase_as structure is always at
 * kbase_device->as[number]. This is used to recover the containing
 * struct kbase_device from a struct kbase_as structure.
 *
 * Therefore, struct kbase_as structures must not be allocated anywhere else.
 */
struct kbase_as {
	int number;

	struct workqueue_struct *pf_wq;
	struct work_struct work_pagefault;
	struct work_struct work_busfault;
	enum kbase_mmu_fault_type fault_type;
	u32 fault_status;
	u64 fault_addr;
	u64 fault_extra_addr;
	struct mutex transaction_mutex;

	struct kbase_mmu_setup current_setup;

	/* BASE_HW_ISSUE_8316  */
	struct workqueue_struct *poke_wq;
	struct work_struct poke_work;
	/** Protected by kbasep_js_device_data::runpool_irq::lock */
	int poke_refcount;
	/** Protected by kbasep_js_device_data::runpool_irq::lock */
	kbase_as_poke_state poke_state;
	struct hrtimer poke_timer;
};

static inline int kbase_as_has_bus_fault(struct kbase_as *as)
{
	return as->fault_type == KBASE_MMU_FAULT_TYPE_BUS;
}

static inline int kbase_as_has_page_fault(struct kbase_as *as)
{
	return as->fault_type == KBASE_MMU_FAULT_TYPE_PAGE;
}

struct kbasep_mem_device {
	atomic_t used_pages;   /* Tracks usage of OS shared memory. Updated
				   when OS memory is allocated/freed. */

};

#define KBASE_TRACE_CODE(X) KBASE_TRACE_CODE_ ## X

enum kbase_trace_code {
	/* IMPORTANT: USE OF SPECIAL #INCLUDE OF NON-STANDARD HEADER FILE
	 * THIS MUST BE USED AT THE START OF THE ENUM */
#define KBASE_TRACE_CODE_MAKE_CODE(X) KBASE_TRACE_CODE(X)
#include "mali_kbase_trace_defs.h"
#undef  KBASE_TRACE_CODE_MAKE_CODE
	/* Comma on its own, to extend the list */
	,
	/* Must be the last in the enum */
	KBASE_TRACE_CODE_COUNT
};

#define KBASE_TRACE_FLAG_REFCOUNT (((u8)1) << 0)
#define KBASE_TRACE_FLAG_JOBSLOT  (((u8)1) << 1)

struct kbase_trace {
	struct timespec timestamp;
	u32 thread_id;
	u32 cpu;
	void *ctx;
	bool katom;
	int atom_number;
	u64 atom_udata[2];
	u64 gpu_addr;
	unsigned long info_val;
#ifdef CONFIG_MALI_EXYNOS_TRACE
	enum kbase_trace_code code;
#else
	u8 code;
#endif
	u8 jobslot;
	u8 refcount;
	u8 flags;
};

/** Event IDs for the power management framework.
 *
 * Any of these events might be missed, so they should not be relied upon to
 * find the precise state of the GPU at a particular time in the
 * trace. Overall, we should get a high percentage of these events for
 * statisical purposes, and so a few missing should not be a problem */
enum kbase_timeline_pm_event {
	/* helper for tests */
	KBASEP_TIMELINE_PM_EVENT_FIRST,

	/** Event reserved for backwards compatibility with 'init' events */
	KBASE_TIMELINE_PM_EVENT_RESERVED_0 = KBASEP_TIMELINE_PM_EVENT_FIRST,

	/** The power state of the device has changed.
	 *
	 * Specifically, the device has reached a desired or available state.
	 */
	KBASE_TIMELINE_PM_EVENT_GPU_STATE_CHANGED,

	/** The GPU is becoming active.
	 *
	 * This event is sent when the first context is about to use the GPU.
	 */
	KBASE_TIMELINE_PM_EVENT_GPU_ACTIVE,

	/** The GPU is becoming idle.
	 *
	 * This event is sent when the last context has finished using the GPU.
	 */
	KBASE_TIMELINE_PM_EVENT_GPU_IDLE,

	/** Event reserved for backwards compatibility with 'policy_change'
	 * events */
	KBASE_TIMELINE_PM_EVENT_RESERVED_4,

	/** Event reserved for backwards compatibility with 'system_suspend'
	 * events */
	KBASE_TIMELINE_PM_EVENT_RESERVED_5,

	/** Event reserved for backwards compatibility with 'system_resume'
	 * events */
	KBASE_TIMELINE_PM_EVENT_RESERVED_6,

	/** The job scheduler is requesting to power up/down cores.
	 *
	 * This event is sent when:
	 * - powered down cores are needed to complete a job
	 * - powered up cores are not needed anymore
	 */
	KBASE_TIMELINE_PM_EVENT_CHANGE_GPU_STATE,

	KBASEP_TIMELINE_PM_EVENT_LAST = KBASE_TIMELINE_PM_EVENT_CHANGE_GPU_STATE,
};

#ifdef CONFIG_MALI_TRACE_TIMELINE
struct kbase_trace_kctx_timeline {
	atomic_t jd_atoms_in_flight;
	u32 owner_tgid;
};

struct kbase_trace_kbdev_timeline {
	/* Note: strictly speaking, not needed, because it's in sync with
	 * kbase_device::jm_slots[]::submitted_nr
	 *
	 * But it's kept as an example of how to add global timeline tracking
	 * information
	 *
	 * The caller must hold kbasep_js_device_data::runpool_irq::lock when
	 * accessing this */
	u8 slot_atoms_submitted[BASE_JM_MAX_NR_SLOTS];

	/* Last UID for each PM event */
	atomic_t pm_event_uid[KBASEP_TIMELINE_PM_EVENT_LAST+1];
	/* Counter for generating PM event UIDs */
	atomic_t pm_event_uid_counter;
	/*
	 * L2 transition state - true indicates that the transition is ongoing
	 * Expected to be protected by pm.power_change_lock */
	bool l2_transitioning;
};
#endif /* CONFIG_MALI_TRACE_TIMELINE */


struct kbasep_kctx_list_element {
	struct list_head link;
	struct kbase_context *kctx;
};

/**
 * Data stored per device for power management.
 *
 * This structure contains data for the power management framework. There is one
 * instance of this structure per device in the system.
 */
struct kbase_pm_device_data {
	/**
	 * The lock protecting Power Management structures accessed outside of
	 * IRQ.
	 *
	 * This lock must also be held whenever the GPU is being powered on or
	 * off.
	 */
	struct mutex lock;

	/** The reference count of active contexts on this device. */
	int active_count;
	/** Flag indicating suspending/suspended */
	bool suspending;

	/* MALI_SEC_INTEGRATION */
	wait_queue_head_t suspending_wait;
	/* Wait queue set when active_count == 0 */
	wait_queue_head_t zero_active_count_wait;

	/**
	 * Bit masks identifying the available shader cores that are specified
	 * via sysfs. One mask per job slot.
	 */
	u64 debug_core_mask[BASE_JM_MAX_NR_SLOTS];
	u64 debug_core_mask_all;

	/**
	 * Lock protecting the power state of the device.
	 *
	 * This lock must be held when accessing the shader_available_bitmap,
	 * tiler_available_bitmap, l2_available_bitmap, shader_inuse_bitmap and
	 * tiler_inuse_bitmap fields of kbase_device, and the ca_in_transition
	 * and shader_poweroff_pending fields of kbase_pm_device_data. It is
	 * also held when the hardware power registers are being written to, to
	 * ensure that two threads do not conflict over the power transitions
	 * that the hardware should make.
	 */
	spinlock_t power_change_lock;

	/**
	 * Callback for initializing the runtime power management.
	 *
	 * @param kbdev The kbase device
	 *
	 * @return 0 on success, else error code
	 */
	 int (*callback_power_runtime_init)(struct kbase_device *kbdev);

	/**
	 * Callback for terminating the runtime power management.
	 *
	 * @param kbdev The kbase device
	 */
	void (*callback_power_runtime_term)(struct kbase_device *kbdev);

	/* Time in milliseconds between each dvfs sample */
	u32 dvfs_period;

	/* Period of GPU poweroff timer */
	ktime_t gpu_poweroff_time;

	/* Number of ticks of GPU poweroff timer before shader is powered off */
	int poweroff_shader_ticks;

	/* Number of ticks of GPU poweroff timer before GPU is powered off */
	int poweroff_gpu_ticks;

	struct kbase_pm_backend_data backend;
};

/**
 * struct kbase_secure_ops - Platform specific functions for GPU secure mode
 * operations
 * @secure_mode_enable:  Callback to enable secure mode on the GPU
 * @secure_mode_disable: Callback to disable secure mode on the GPU
 */
struct kbase_secure_ops {
	/**
	 * secure_mode_enable() - Enable secure mode on the GPU
	 * @kbdev:	The kbase device
	 *
	 * Return: 0 on success, non-zero on error
	 */
	int (*secure_mode_enable)(struct kbase_device *kbdev);

	/**
	 * secure_mode_disable() - Disable secure mode on the GPU
	 * @kbdev:	The kbase device
	 *
	 * Return: 0 on success, non-zero on error
	 */
	int (*secure_mode_disable)(struct kbase_device *kbdev);

/* MALI_SEC_SECURE_RENDERING */
	/** Platform specific function for initializing the secure region
	 *
	 * Return 0 if successful, otherwise error
	 */
	 int (*secure_mode_init)(struct kbase_device *kbdev);

/* MALI_SEC_SECURE_RENDERING */
	/** Platform specific function for setting the Memory to secure mode.
	 *
	 * Return 0 if successful, otherwise error
	 */
	int (*secure_mem_enable)(struct kbase_device *kbdev, int ion_fd, u64 flags, struct kbase_va_region *reg);

/* MALI_SEC_SECURE_RENDERING */
	/** Platform specific function for setting the Memory back to normal mode.
	 *
	 * Return 0 if successful, otherwise error
	 */
	int (*secure_mem_disable)(struct kbase_device *kbdev, struct kbase_va_region *reg);
};

/* MALI_SEC_SECURE_RENDERING */
typedef struct sec_sr_resources {
	u64             secure_flags_crc_asp;

	phys_addr_t     secure_crc_phys;
	size_t          secure_crc_sizes;
} sec_sr_resources;

#ifdef MALI_SEC_HWCNT
typedef struct hwc_resources {
	u64 arith_words;
	u64 ls_issues;
	u64 tex_issues;
	u64 tripipe_active;
#ifdef MALI_SEC_HWCNT_VERT
	u64 gpu_active;
	u64 js0_active;
	u64 tiler_active;
	u64 external_read_bits;
#endif
} hwc_resources;
#endif

/**
 * struct kbase_mem_pool - Page based memory pool for kctx/kbdev
 * @kbdev:     Kbase device where memory is used
 * @cur_size:  Number of free pages currently in the pool (may exceed @max_size
 *             in some corner cases)
 * @max_size:  Maximum number of free pages in the pool
 * @pool_lock: Lock protecting the pool - must be held when modifying @cur_size
 *             and @page_list
 * @page_list: List of free pages in the pool
 * @reclaim:   Shrinker for kernel reclaim of free pages
 * @next_pool: Pointer to next pool where pages can be allocated when this pool
 *             is empty. Pages will spill over to the next pool when this pool
 *             is full. Can be NULL if there is no next pool.
 */
struct kbase_mem_pool {
	struct kbase_device *kbdev;
	size_t              cur_size;
	size_t              max_size;
	spinlock_t          pool_lock;
	struct list_head    page_list;
	struct shrinker     reclaim;

	struct kbase_mem_pool *next_pool;
};


#define DEVNAME_SIZE	16

struct kbase_device {
	s8 slot_submit_count_irq[BASE_JM_MAX_NR_SLOTS];

	u32 hw_quirks_sc;
	u32 hw_quirks_tiler;
	u32 hw_quirks_mmu;
	u32 hw_quirks_jm;

	struct list_head entry;
	struct device *dev;
	struct miscdevice mdev;
	u64 reg_start;
	size_t reg_size;
	void __iomem *reg;

	struct {
		int irq;
		int flags;
	} irqs[3];
#ifdef CONFIG_HAVE_CLK
	struct clk *clock;
#endif
#ifdef CONFIG_REGULATOR
	struct regulator *regulator;
#endif
	char devname[DEVNAME_SIZE];

#ifdef CONFIG_MALI_NO_MALI
	void *model;
	struct kmem_cache *irq_slab;
	struct workqueue_struct *irq_workq;
	atomic_t serving_job_irq;
	atomic_t serving_gpu_irq;
	atomic_t serving_mmu_irq;
	spinlock_t reg_op_lock;
#endif	/* CONFIG_MALI_NO_MALI */

	struct kbase_pm_device_data pm;
	struct kbasep_js_device_data js_data;
	struct kbase_mem_pool mem_pool;
	struct kbasep_mem_device memdev;
	struct kbase_mmu_mode const *mmu_mode;

	struct kbase_as as[BASE_MAX_NR_AS];

	spinlock_t mmu_mask_change;

	struct kbase_gpu_props gpu_props;

	/** List of SW workarounds for HW issues */
	unsigned long hw_issues_mask[(BASE_HW_ISSUE_END + BITS_PER_LONG - 1) / BITS_PER_LONG];
	/** List of features available */
	unsigned long hw_features_mask[(BASE_HW_FEATURE_END + BITS_PER_LONG - 1) / BITS_PER_LONG];

	/* Bitmaps of cores that are currently in use (running jobs).
	 * These should be kept up to date by the job scheduler.
	 *
	 * pm.power_change_lock should be held when accessing these members.
	 *
	 * kbase_pm_check_transitions_nolock() should be called when bits are
	 * cleared to update the power management system and allow transitions to
	 * occur. */
	u64 shader_inuse_bitmap;

	/* Refcount for cores in use */
	u32 shader_inuse_cnt[64];

	/* Bitmaps of cores the JS needs for jobs ready to run */
	u64 shader_needed_bitmap;

	/* Refcount for cores needed */
	u32 shader_needed_cnt[64];

	u32 tiler_inuse_cnt;

	u32 tiler_needed_cnt;

	/* struct for keeping track of the disjoint information
	 *
	 * The state  is > 0 if the GPU is in a disjoint state. Otherwise 0
	 * The count is the number of disjoint events that have occurred on the GPU
	 */
	struct {
		atomic_t count;
		atomic_t state;
	} disjoint_event;

	/* Refcount for tracking users of the l2 cache, e.g. when using hardware counter instrumentation. */
	u32 l2_users_count;

	/* Bitmaps of cores that are currently available (powered up and the power policy is happy for jobs to be
	 * submitted to these cores. These are updated by the power management code. The job scheduler should avoid
	 * submitting new jobs to any cores that are not marked as available.
	 *
	 * pm.power_change_lock should be held when accessing these members.
	 */
	u64 shader_available_bitmap;
	u64 tiler_available_bitmap;
	u64 l2_available_bitmap;

	u64 shader_ready_bitmap;
	u64 shader_transitioning_bitmap;

	s8 nr_hw_address_spaces;			  /**< Number of address spaces in the GPU (constant after driver initialisation) */
	s8 nr_user_address_spaces;			  /**< Number of address spaces available to user contexts */

	/* Structure used for instrumentation and HW counters dumping */
	struct kbase_hwcnt {
		/* The lock should be used when accessing any of the following members */
		spinlock_t lock;

		struct kbase_context *kctx;
		u64 addr;

		struct kbase_context *suspended_kctx;
		struct kbase_uk_hwcnt_setup suspended_state;

		struct kbase_instr_backend backend;

#ifdef MALI_SEC_HWCNT
		s32 hwcnt_fd;
		struct mutex mlock;
		bool is_hwcnt_attach;
		bool is_hwcnt_enable;
		bool is_hwcnt_gpr_enable;
		bool is_hwcnt_force_stop;
		u32 timeout;
		struct hwc_resources resources;
		struct hwc_resources resources_log;
		u32 cnt_for_bt_start;
		u32 cnt_for_bt_stop;
#endif
	} hwcnt;

	struct kbase_vinstr_context *vinstr_ctx;

	/*value to be written to the irq_throttle register each time an irq is served */
	atomic_t irq_throttle_cycles;

#if KBASE_TRACE_ENABLE
	spinlock_t              trace_lock;
	u16                     trace_first_out;
	u16                     trace_next_in;
	struct kbase_trace            *trace_rbuf;
#endif

	/* This is used to override the current job scheduler values for
	 * JS_SCHEDULING_PERIOD_NS
	 * JS_SOFT_STOP_TICKS
	 * JS_SOFT_STOP_TICKS_CL
	 * JS_HARD_STOP_TICKS_SS
	 * JS_HARD_STOP_TICKS_CL
	 * JS_HARD_STOP_TICKS_DUMPING
	 * JS_RESET_TICKS_SS
	 * JS_RESET_TICKS_CL
	 * JS_RESET_TICKS_DUMPING.
	 *
	 * These values are set via the js_timeouts sysfs file.
	 */
	u32 js_scheduling_period_ns;
	int js_soft_stop_ticks;
	int js_soft_stop_ticks_cl;
	int js_hard_stop_ticks_ss;
	int js_hard_stop_ticks_cl;
	int js_hard_stop_ticks_dumping;
	int js_reset_ticks_ss;
	int js_reset_ticks_cl;
	int js_reset_ticks_dumping;
	bool js_timeouts_updated;

	u32 reset_timeout_ms;

	struct mutex cacheclean_lock;

	/* Platform specific private data to be accessed by mali_kbase_config_xxx.c only */
	void *platform_context;

	/* List of kbase_contexts created */
	struct list_head        kctx_list;
	struct mutex            kctx_list_lock;

#ifdef CONFIG_PM_DEVFREQ
	struct devfreq_dev_profile devfreq_profile;
	struct devfreq *devfreq;
	unsigned long current_freq;
	unsigned long current_voltage;
#ifdef CONFIG_DEVFREQ_THERMAL
	struct devfreq_cooling_device *devfreq_cooling;
#endif
#endif

	struct kbase_ipa_context *ipa_ctx;

#ifdef CONFIG_MALI_TRACE_TIMELINE
	struct kbase_trace_kbdev_timeline timeline;
#endif

	/*
	 * Control for enabling job dump on failure, set when control debugfs
	 * is opened.
	 */
	bool job_fault_debug;

#ifdef CONFIG_DEBUG_FS
	/* directory for debugfs entries */
	struct dentry *mali_debugfs_directory;
	/* Root directory for per context entry */
	struct dentry *debugfs_ctx_directory;
#ifndef MALI_SEC_INTEGRATION
	/* debugfs entry for trace */
	struct dentry *trace_dentry;
#endif /* MALI_SEC_INTEGRATION */
	/* failed job dump, used for separate debug process */
	wait_queue_head_t job_fault_wq;
	wait_queue_head_t job_fault_resume_wq;
	struct workqueue_struct *job_fault_resume_workq;
	struct list_head job_fault_event_list;
	spinlock_t job_fault_event_lock;
	struct kbase_context *kctx_fault;

#if !MALI_CUSTOMER_RELEASE
	/* Per-device data for register dumping interface */
	struct {
		u16 reg_offset; /* Offset of a GPU_CONTROL register to be
				   dumped upon request */
	} regs_dump_debugfs_data;
#endif /* !MALI_CUSTOMER_RELEASE */
#endif /* CONFIG_DEBUG_FS */

	/* fbdump profiling controls set by gator */
	u32 kbase_profiling_controls[FBDUMP_CONTROL_MAX];


#if MALI_CUSTOMER_RELEASE == 0
	/* Number of jobs that are run before a job is forced to fail and
	 * replay. May be KBASEP_FORCE_REPLAY_DISABLED, to disable forced
	 * failures. */
	int force_replay_limit;
	/* Count of jobs between forced failures. Incremented on each job. A
	 * job is forced to fail once this is greater than or equal to
	 * force_replay_limit. */
	int force_replay_count;
	/* Core requirement for jobs to be failed and replayed. May be zero. */
	base_jd_core_req force_replay_core_req;
	/* true if force_replay_limit should be randomized. The random
	 * value will be in the range of 1 - KBASEP_FORCE_REPLAY_RANDOM_LIMIT.
	 */
	bool force_replay_random;
#endif

	/* Total number of created contexts */
	atomic_t ctx_num;

	struct kbase_hwaccess_data hwaccess;

	/* Count of page/bus faults waiting for workqueues to process */
	atomic_t faults_pending;

	/* true if GPU is powered off or power off operation is in progress */
	bool poweroff_pending;


	/* defaults for new context created for this device */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
	bool infinite_cache_active_default;
#else
	u32 infinite_cache_active_default;
#endif
	size_t mem_pool_max_size_default;

	/* system coherency mode  */
	u32 system_coherency;
	/* Flag to track when cci snoops have been enabled on the interface */
	bool cci_snoop_enabled;

	/* SMC function IDs to call into Trusted firmware to enable/disable
	 * cache snooping. Value of 0 indicates that they are not used
	 */
	u32 snoop_enable_smc;
	u32 snoop_disable_smc;

	/* Secure operations */
	struct kbase_secure_ops *secure_ops;

	/*
	 * true when GPU is put into secure mode
	 */
	bool secure_mode;

	/*
	 * true if secure mode is supported
	 */
	bool secure_mode_support;

	/*
	 * MALI_SEC_SECURE_RENDERING
	 *
	 * information for secure ASP setting
	 *
	 */
	sec_sr_resources sec_sr_info;

#ifdef CONFIG_MALI_DEBUG
	wait_queue_head_t driver_inactive_wait;
	bool driver_inactive;
#endif /* CONFIG_MALI_DEBUG */

#ifdef CONFIG_MALI_FPGA_BUS_LOGGER
	/*
	 * Bus logger integration.
	 */
	struct bus_logger_client *buslogger;
#endif
	/* Boolean indicating if an IRQ flush during reset is in progress. */
	bool irq_reset_flush;

	/* list of inited sub systems. Used during terminate/error recovery */
	u32 inited_subsys;

        /* MALI_SEC_INTEGRATION */
	struct kbase_vendor_callbacks *vendor_callbacks;
};

/* JSCTX ringbuffer size will always be a power of 2. The idx shift must be:
   - >=2 (buffer size -> 4)
   - <= 9 (buffer size 2^(9-1)=256) (technically, 10 works for the ringbuffer
				but this is unnecessary as max atoms is 256)
 */
#define JSCTX_RB_IDX_SHIFT (8U)
#if ((JSCTX_RB_IDX_SHIFT < 2) || ((3 * JSCTX_RB_IDX_SHIFT) >= 32))
#error "Invalid ring buffer size for 32bit atomic."
#endif
#define JSCTX_RB_SIZE (1U << (JSCTX_RB_IDX_SHIFT - 1U)) /* 1 bit for overflow */
#define JSCTX_RB_SIZE_STORE (1U << JSCTX_RB_IDX_SHIFT)
#define JSCTX_RB_MASK (JSCTX_RB_SIZE - 1U)
#define JSCTX_RB_MASK_STORE (JSCTX_RB_SIZE_STORE - 1U)

#define JSCTX_WR_OFFSET         (0U)
#define JSCTX_RN_OFFSET         (JSCTX_WR_OFFSET   + JSCTX_RB_IDX_SHIFT)
#define JSCTX_RD_OFFSET         (JSCTX_RN_OFFSET + JSCTX_RB_IDX_SHIFT)

/**
 * struct jsctx_rb_entry - Ringbuffer entry in &struct jsctx_queue.
 * @atom_id: Atom ID
 */
struct jsctx_rb_entry {
	u16 atom_id;
};

/**
 * struct jsctx_queue - JS context atom queue, containing both ring buffer and linked list.
 * @entries:     Array of size %JSCTX_RB_SIZE which holds the &struct
 *               kbase_jd_atom pointers which make up the contents of the ring
 *               buffer.
 * @indicies:    An atomic variable containing indicies for the ring buffer.
 *               Indicies are of size JSCTX_RB_IDX_SHIFT.
 *               The following are contained:
 *                - WR_IDX - Write index. Index of the NEXT slot to be written.
 *                - RN_IDX - Running index. Index of the tail of the list.
 *                           This is the atom that has been running the longest.
 *                - RD_IDX - Read index. Index of the next atom to be pulled.
 * @queue_head:  Head item of the linked list queue.
 *
 * Locking:
 * The linked list assumes jctx.lock is held.
 * The ringbuffer serves as an intermediary between irq context and non-irq
 * context, without the need for the two to share any lock. irq context can
 * pull (and unpull) and only requires the runpool_irq.lock. While non-irq
 * context can add and remove and only requires holding only jctx.lock.
 * Error handling affecting both, or the whole ringbuffer in general, must
 * hold both locks or otherwise ensure (f.ex deschedule/kill) only that thread
 * is accessing the buffer.
 * This means that RD_IDX is updated by irq-context (pull and unpull) and must
 * hold runpool_irq.lock. While WR_IDX (add) and RN_IDX (remove) is updated by
 * non-irq context and must hold jctx.lock.
 * Note that pull (or sister function peek) must also access WR_IDX to ensure
 * there is free space in the buffer, this is ok as WR_IDX is only increased.
 * A similar situation is apparent with unpull and RN_IDX, but only one atom
 * (already pulled) can cause either remove or unpull, so this will never
 * conflict.
 *
 * &struct jsctx_queue is a queue of &struct kbase_jd_atom,
 * part ringbuffer and part linked list.
 */
struct jsctx_queue {
	struct jsctx_rb_entry entries[JSCTX_RB_SIZE];

	atomic_t indicies;

	struct list_head queue_head;
};





#define KBASE_API_VERSION(major, minor) ((((major) & 0xFFF) << 20)  | \
					 (((minor) & 0xFFF) << 8) | \
					 ((0 & 0xFF) << 0))

struct kbase_context {
	struct file *filp;
	struct kbase_device *kbdev;
	int id; /* System wide unique id */
	unsigned long api_version;
	phys_addr_t pgd;
	struct list_head event_list;
	struct list_head event_coalesce_list;
	struct mutex event_mutex;
	atomic_t event_closed;
	struct workqueue_struct *event_workq;
	atomic_t event_count;
	int event_coalesce_count;

	bool is_compat;

	atomic_t                setup_complete;
	atomic_t                setup_in_progress;

	u64 *mmu_teardown_pages;

	struct page *aliasing_sink_page;

	struct mutex            mmu_lock;
	struct mutex            reg_lock; /* To be converted to a rwlock? */
	struct rb_root          reg_rbtree; /* Red-Black tree of GPU regions (live regions) */

	unsigned long    cookies;
	struct kbase_va_region *pending_regions[BITS_PER_LONG];

	wait_queue_head_t event_queue;
	pid_t tgid;
	pid_t pid;

	struct kbase_jd_context jctx;
	atomic_t used_pages;
	atomic_t         nonmapped_pages;

	struct kbase_mem_pool mem_pool;

	struct shrinker         reclaim;
	struct list_head        evict_list;
	struct mutex            evict_lock;

	struct list_head waiting_soft_jobs;
	spinlock_t waiting_soft_jobs_lock;
#ifdef CONFIG_KDS
	struct list_head waiting_kds_resource;
#endif
#ifdef CONFIG_MALI_DMA_FENCE
	struct {
		struct list_head waiting_resource;
		struct workqueue_struct *wq;
	} dma_fence;
#endif /* CONFIG_MALI_DMA_FENCE */
	/** This is effectively part of the Run Pool, because it only has a valid
	 * setting (!=KBASEP_AS_NR_INVALID) whilst the context is scheduled in
	 *
	 * The kbasep_js_device_data::runpool_irq::lock must be held whilst accessing
	 * this.
	 *
	 * If the context relating to this as_nr is required, you must use
	 * kbasep_js_runpool_retain_ctx() to ensure that the context doesn't disappear
	 * whilst you're using it. Alternatively, just hold the kbasep_js_device_data::runpool_irq::lock
	 * to ensure the context doesn't disappear (but this has restrictions on what other locks
	 * you can take whilst doing this) */
	int as_nr;

	/* NOTE:
	 *
	 * Flags are in jctx.sched_info.ctx.flags
	 * Mutable flags *must* be accessed under jctx.sched_info.ctx.jsctx_mutex
	 *
	 * All other flags must be added there */
	spinlock_t         mm_update_lock;
	struct mm_struct *process_mm;
	/* End of the SAME_VA zone */
	u64 same_va_end;

#ifdef CONFIG_MALI_TRACE_TIMELINE
	struct kbase_trace_kctx_timeline timeline;
#endif
#ifdef CONFIG_DEBUG_FS
	/* Content of mem_profile file */
	char *mem_profile_data;
	/* Size of @c mem_profile_data */
	size_t mem_profile_size;
	/* Mutex guarding memory profile state */
	struct mutex mem_profile_lock;
	/* Memory profile file created */
	bool mem_profile_initialized;
	struct dentry *kctx_dentry;

	/* for job fault debug */
	unsigned int *reg_dump;
	atomic_t job_fault_count;
	/* This list will keep the following atoms during the dump
	 * in the same context
	 */
	struct list_head job_fault_resume_event_list;

#endif /* CONFIG_DEBUG_FS */

	struct jsctx_queue jsctx_queue
		[KBASE_JS_ATOM_SCHED_PRIO_COUNT][BASE_JM_MAX_NR_SLOTS];

	/* Number of atoms currently pulled from this context */
	atomic_t atoms_pulled;
	/* Number of atoms currently pulled from this context, per slot */
	atomic_t atoms_pulled_slot[BASE_JM_MAX_NR_SLOTS];
	/* true if last kick() caused atoms to be pulled from this context */
	bool pulled;
	/* true if infinite cache is to be enabled for new allocations. Existing
	 * allocations will not change. bool stored as a u32 per Linux API */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
	bool infinite_cache_active;
#else
	u32 infinite_cache_active;
#endif
	/* Bitmask of slots that can be pulled from */
	u32 slots_pullable;

	/* true if address space assignment is pending */
	bool as_pending;

	/* Backend specific data */
	struct kbase_context_backend backend;

	/* Work structure used for deferred ASID assignment */
	struct work_struct work;

	/* Only one userspace vinstr client per kbase context */
	struct kbase_vinstr_client *vinstr_cli;
	struct mutex vinstr_cli_lock;

	/* Must hold queue_mutex when accessing */
	bool ctx_active;

	/* List of completed jobs waiting for events to be posted */
	struct list_head completed_jobs;
	/* Number of work items currently pending on job_done_wq */
	atomic_t work_count;

	/* true if context is counted in kbdev->js_data.nr_contexts_runnable */
	bool ctx_runnable_ref;

	/* Waiting soft-jobs will fail when this timer expires */
	struct hrtimer soft_event_timeout;

	/* JIT allocation management */
	struct kbase_va_region *jit_alloc[255];
	struct list_head jit_active_head;
	struct list_head jit_pool_head;
	struct list_head jit_destroy_head;
	struct mutex jit_lock;
	struct work_struct jit_work;

        /* External sticky resource management */
        struct list_head ext_res_meta_head;

        /* MALI_SEC_INTEGRATION */
	int ctx_status;
	bool ctx_need_qos;
	char name[CTX_NAME_SIZE];

	/* MALI_SEC_SECURE_RENDERING */
	bool enabled_TZASC;

	/* MALI_SEC_INTEGRATION */
	bool destroying_context;
	atomic_t mem_profile_showing_state;
	wait_queue_head_t mem_profile_wait;
};

/**
 * struct kbase_ctx_ext_res_meta - Structure which binds an external resource
 *                                 to a @kbase_context.
 * @ext_res_node:                  List head for adding the metadata to a
 *                                 @kbase_context.
 * @alloc:                         The physical memory allocation structure
 *                                 which is mapped.
 * @gpu_addr:                      The GPU virtual address the resource is
 *                                 mapped to.
 *
 * External resources can be mapped into multiple contexts as well as the same
 * context multiple times.
 * As kbase_va_region itself isn't refcounted we can't attach our extra
 * information to it as it could be removed under our feet leaving external
 * resources pinned.
 * This metadata structure binds a single external resource to a single
 * context, ensuring that per context mapping is tracked separately so it can
 * be overridden when needed and abuses by the application (freeing the resource
 * multiple times) don't effect the refcount of the physical allocation.
 */
struct kbase_ctx_ext_res_meta {
        struct list_head ext_res_node;
        struct kbase_mem_phy_alloc *alloc;
        u64 gpu_addr;
};

enum kbase_reg_access_type {
	REG_READ,
	REG_WRITE
};

enum kbase_share_attr_bits {
	/* (1ULL << 8) bit is reserved */
	SHARE_BOTH_BITS = (2ULL << 8),	/* inner and outer shareable coherency */
	SHARE_INNER_BITS = (3ULL << 8)	/* inner shareable coherency */
};

/**
 * kbase_device_is_cpu_coherent - Returns if the device is CPU coherent.
 * @kbdev: kbase device
 *
 * Return: true if the device access are coherent, false if not.
 */
static inline bool kbase_device_is_cpu_coherent(struct kbase_device *kbdev)
{
	if ((kbdev->system_coherency == COHERENCY_ACE_LITE) ||
			(kbdev->system_coherency == COHERENCY_ACE))
		return true;

	return false;
}

/* Conversion helpers for setting up high resolution timers */
#define HR_TIMER_DELAY_MSEC(x) (ns_to_ktime((x)*1000000U))
#define HR_TIMER_DELAY_NSEC(x) (ns_to_ktime(x))

/* Maximum number of loops polling the GPU for a cache flush before we assume it must have completed */
#define KBASE_CLEAN_CACHE_MAX_LOOPS     100000
/* Maximum number of loops polling the GPU for an AS command to complete before we assume the GPU has hung */
#define KBASE_AS_INACTIVE_MAX_LOOPS     100000

/* Maximum number of times a job can be replayed */
#define BASEP_JD_REPLAY_LIMIT 15

/* JobDescriptorHeader - taken from the architecture specifications, the layout
 * is currently identical for all GPU archs. */
struct job_descriptor_header {
	u32 exception_status;
	u32 first_incomplete_task;
	u64 fault_pointer;
	u8 job_descriptor_size : 1;
	u8 job_type : 7;
	u8 job_barrier : 1;
	u8 _reserved_01 : 1;
	u8 _reserved_1 : 1;
	u8 _reserved_02 : 1;
	u8 _reserved_03 : 1;
	u8 _reserved_2 : 1;
	u8 _reserved_04 : 1;
	u8 _reserved_05 : 1;
	u16 job_index;
	u16 job_dependency_index_1;
	u16 job_dependency_index_2;
	union {
		u64 _64;
		u32 _32;
	} next_job;
};

#endif				/* _KBASE_DEFS_H_ */
